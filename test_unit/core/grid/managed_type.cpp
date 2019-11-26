#include "gtest/gtest.h"
#include "types_util/types_util.hpp"
#include "core/grid/managed_type.hpp"
//#include "core/grid/common.hpp"
#include "core/mock_management_type.hpp"

namespace {

using namespace logicker::core::grid;

namespace type = logicker::type;

using MValueTypeI = ManagedValueType<int, MockReadOnlyManagementType<int>>;
using MValueTypeF = ManagedValueType<float, MockReadOnlyManagementType<float>>;
using MMapType = ManagedMapType<bool, MValueTypeI, MValueTypeF>;
using InvalidMMapType = ManagedMapType<bool, int>;

//ManagedValueType:
//zna svuj typ hodnoty
static_assert(std::is_same_v<typename MValueTypeI::valueType, int>);
//zna svuj typ managementu
static_assert(std::is_same_v<typename MValueTypeI::managementType, MockReadOnlyManagementType<int>>);
//
//ManagedMapType:
//nejde vytvorit s typem, kterej neni ManagedValue
//InvalidMMapType invMap{};//this does not compile
//zna svuj typ klice
static_assert(std::is_same_v<typename MMapType::keyType, bool>);
//zna sve typy managed hodnot (jako tuple)
static_assert(
    std::is_same_v<typename MMapType::valueTypesTuple, std::tuple<MValueTypeI, MValueTypeF>>
    || std::is_same_v<typename MMapType::valueTypesTuple, std::tuple<MValueTypeF, MValueTypeI>>);
//zna sve typy managed hodnot (jako set)
static_assert(type::is_same_set<typename MMapType::valueTypesSet, type::makeSetT<MValueTypeI, MValueTypeF>>);

}
