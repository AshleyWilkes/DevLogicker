#include "gtest/gtest.h"
#include "core/grid/managed_id.hpp"
#include "core/grid/common.hpp"

namespace {

namespace type = logicker::type;

char valueStr[] = "value";
using MValueId = ManagedValueId<valueStr, int>;

char mapStr[] = "map";
using MMapIdIDF = ManagedMapId<mapStr, int, double, float>;
using MMapIdBIF = ManagedMapId<mapStr, bool, int, float>;

using InvalidMIdsSet = ManagedIdsSet<int>;
using MIdsSet = ManagedIdsSet<MValueId, MMapIdIDF>;

//ManagedValueId:
//test, ze vytvorene id zna sve jmeno
static_assert(MValueId::name == valueStr);
//test, ze vytvorene id zna svuj typ
static_assert(std::is_same_v<typename MValueId::type, int>);

//ManagedMapId:
//test, ze zna sve jmeno
static_assert(MMapIdIDF::name == mapStr);
//test, ze zna svuj typ klice
static_assert(std::is_same_v<typename MMapIdIDF::keyType, int>);
//test, ze zna sve typy hodnot (jako tuple)
static_assert(
    std::is_same_v<typename MMapIdIDF::valueTypesTuple, std::tuple<double, float>>
    || std::is_same_v<typename MMapIdIDF::valueTypesTuple, std::tuple<float, double>>);
//test, ze zna sve typy hodnot (jako set)
static_assert(type::is_same_set<typename MMapIdIDF::valueTypesSet, type::makeSetT<float, double>>);

//ManagedIdsSet:
//nejde vytvorit s typem, kterej neni ManagedId
//InvalidMIdsSet invSet{};//this does not compile
//
//jde vytvorit s ManagedXXXIds -- to se overi nasledujicim assertem, ten zpusobi instancializaci MIdsSetu
//umi vratit sve Ids jako tuple
static_assert(
    std::is_same_v<typename MIdsSet::tuple, std::tuple<MValueId, MMapIdIDF>>
    || std::is_same_v<typename MIdsSet::tuple, std::tuple<MMapIdIDF, MValueId>>);
//umi vratit sve Ids jako Set
static_assert(type::is_same_set<typename MIdsSet::set, type::makeSetT<MValueId, MMapIdIDF>>);

char valueStr2[] = "value";
using MValueId2 = ManagedValueId<valueStr2, int>;

char valueABCStr[] = "valueABC";
using MValueId3 = ManagedValueId<valueABCStr, int>;

using MValueId4 = ManagedValueId<valueStr, float>;

//test, ze MValueId je svym subId
static_assert(is_sub_MId_v<MValueId, MValueId>);
//test, ze MValueId je subId jineho MValueId se stejnym jmenem a stejnou hodnotou
static_assert(is_sub_MId_v<MValueId, MValueId2>);
//test, ze MValueId neni subId jineho MValueId (jine jmeno)
//static_assert(! is_sub_MId_v<MValueId, MValueId3>);//ted neni podstatne
//test, ze MValueId neni subId jineho MValueId (jina hodnota)
static_assert(! is_sub_MId_v<MValueId, MValueId4>);

char mapStr2[] = "map";
using MMapIdIDF2 = ManagedMapId<mapStr2, int, double, float>;

using MMapIdIBDF = ManagedMapId<mapStr, int, bool, double, float>;

char mapABCStr[] = "mapABC";
using MMapIdIDF3 = ManagedMapId<mapABCStr, int, double, float>;

using MMapIdFDF = ManagedMapId<mapStr, float, double, float>;

using MMapIdID = ManagedMapId<mapStr, int, double>;

//test, ze MMapId je svym subId
static_assert(is_sub_MId_v<MMapIdIDF, MMapIdIDF>);
//test, ze MMapId je subId jineho MMapId se stejnym obsahem
static_assert(is_sub_MId_v<MMapIdIDF, MMapIdIDF2>);
//test, ze MMapId je subId jineho MMapId se stejnym jmenem, klicem a nadmnozinou valueTypes
static_assert(is_sub_MId_v<MMapIdIDF, MMapIdIBDF>);
//test, ze MMapId neni subId jineho MMapId (jine jmeno, jinak stejne)
//static_assert(! is_sub_MId_v<MMapIdIDF, MMapIdIDF3>);//ted neni podstatne
//test, ze MMapId neni subId jineho MMapId (jiny klic, jinak stejne)
static_assert(! is_sub_MId_v<MMapIdIDF, MMapIdFDF>);
//test, ze MMapId neni subId jineho MMapId (podmnozina valueTypes, jinak stejne)
static_assert(! is_sub_MId_v<MMapIdIDF, MMapIdID>);

//test, ze MValueId neni subId MMapId
static_assert(! is_sub_MId_v<MValueId, MMapIdIDF>);
//test, ze MMapId neni subId MValueId
static_assert(! is_sub_MId_v<MMapIdIDF, MValueId>);

}
