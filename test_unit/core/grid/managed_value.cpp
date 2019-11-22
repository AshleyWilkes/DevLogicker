#include "gtest/gtest.h"
#include "core/grid/managed_value.hpp"
#include "core/grid/managed_type.hpp"
#include "core/grid/common.hpp"

namespace {

using MValueTypeI = ManagedValueType<int, typename DummyManagementType::type<int>>;

using MValue = ManagedValue<MValueTypeI>;
using MValue2 = ManagedValue<int, typename DummyManagementType::type<int>>;

//ManagedValue:
//templatovan na jedne strane MValueTypem a na druhe strane odpovidajici dvojici <type, managementType>
//  vede na stejny typ
static_assert(std::is_same_v<MValue, MValue2>);

TEST(ManagedValue, SetAndGet) {
  //lze vytvorit konstruktorem se stejnymi parametry jako ma konstruktor ManagementTypu
  MValue mValue{ 42 };
  //umi vratit const referenci na objekt typu ManagementType<typHodnoty>
  const auto& valueInstance = mValue.getInstance();
  EXPECT_EQ( valueInstance.get(), 42 );
}

//lze porovnavat na inkluzi (operator <=):
//tj. existuje sablona
//template<typename MValue1, typename MValue2>
//bool operator<=( const MValue1&, const MValue2& );
//a ta zavola na obou operandech getValueSet(), coz vrati std::set<T>, a tyhle sety porovnam na subset
//  (manualne prochazet oba sety a hledat :-(
//
//cili k otestovani staci mit dve MValues, ktery daji sety v ostry inkluzi a zavolat porovnani
//  na obe strany

}
