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

}
