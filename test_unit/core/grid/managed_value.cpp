#include "gtest/gtest.h"
#include "core/grid/grid.hpp"
#include "core/grid/common.hpp"

namespace {

using MValueTypeI = ManagedValueType<int, DummyManagementType<int>>;

using MValue = ManagedValue<MValueTypeI>;
using MValue2 = ManagedValue<int, DummyManagementType<int>>;

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

}
