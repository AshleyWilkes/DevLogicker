#include "gtest/gtest.h"
#include "core/grid/managed_value.hpp"
#include "core/grid/managed_type.hpp"
#include "core/mock_management_type.hpp"

namespace {

using namespace logicker::core::grid;

using MValueTypeI = ManagedValueType<int, MockRangeManagementType<4, int>>;

using MValue = ManagedValue<MValueTypeI>;
using MValue2 = ManagedValue<int, MockRangeManagementType<4, int>>;

//ManagedValue:
//templatovan na jedne strane MValueTypem a na druhe strane odpovidajici dvojici <type, managementType>
//  vede na stejny typ
static_assert(std::is_same_v<MValue, MValue2>);

TEST(ManagedValue, SetAndGet) {
  //lze vytvorit konstruktorem se stejnymi parametry jako ma konstruktor ManagementTypu
  MValue mValue{ 42 };
  //umi vratit referenci na objekt typu ManagementType<typHodnoty>
  auto& valueInstance = mValue.getInstance();
  EXPECT_CALL( valueInstance.getMock(), mockedGet() );
  EXPECT_CALL( valueInstance.getMock(), mockedSet( 44 ) );
  valueInstance.set( 44 );
  EXPECT_EQ( valueInstance.get(), 44 );
}

TEST(ManagedValue, OperatorLEWhenValueErasedBeforeEnd) {
  MValue mValue1{ 15 };
  MValue mValue2{ 15 };
  auto& inst1 = mValue1.getInstance();
  auto& inst2 = mValue2.getInstance();
  EXPECT_CALL( inst1.getMock(), mockedErase( 16 ) );
  EXPECT_CALL( inst1.getMock(), mockedGetValueSet() ).Times( 2 );
  EXPECT_CALL( inst2.getMock(), mockedGetValueSet() ).Times( 2 );
  inst1.erase( 16 );
  EXPECT_LE( mValue1, mValue2);
  EXPECT_FALSE( mValue2 <= mValue1 );
}

TEST(ManagedValue, OperatorLEWhenValueErasedAtTheEnd) {
  MValue mValue1{ 15 };
  MValue mValue2{ 15 };
  auto& inst1 = mValue1.getInstance();
  auto& inst2 = mValue2.getInstance();
  EXPECT_CALL( inst1.getMock(), mockedErase( 18 ) );
  EXPECT_CALL( inst1.getMock(), mockedGetValueSet() ).Times( 2 );
  EXPECT_CALL( inst2.getMock(), mockedGetValueSet() ).Times( 2 );
  inst1.erase( 18 );
  EXPECT_LE( mValue1, mValue2);
  EXPECT_FALSE( mValue2 <= mValue1 );
}

}
