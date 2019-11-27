#include "gtest/gtest.h"
#include "core/grid/managed_type.hpp"
#include "core/grid/managed_map.hpp"
#include "core/mock_management_type.hpp"

namespace {

using namespace logicker::core::grid;

using MValueTypeI = ManagedValueType<int, MockRangeManagementType<4, int>>;
using MValueTypeB = ManagedValueType<bool, MockReadOnlyManagementType<bool>>;
using MValueTypeF = ManagedValueType<float, MockRangeManagementType<5, float>>;

using MMapType = ManagedMapType<bool, MValueTypeI, MValueTypeB>;

using MMap = ManagedMap<MMapType>;

//ManagedMap:
TEST(ManagedMap, SetAndGetSuccess) {
  //jde vytvorit, addnout hodnotu, getnout ji
  MMap mMap;
  mMap.add<MValueTypeI>( true );
  mMap.add<MValueTypeB>( false );
  mMap.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap.get<MValueTypeI>( true ).getInstance().set( 42 );
  mMap.get<MValueTypeB>( false ).getInstance().init( false );
  EXPECT_EQ( mMap.get<MValueTypeI>( true ).getInstance().get(), 42);
  EXPECT_EQ( mMap.get<MValueTypeB>( false ).getInstance().get(), false );
  const auto& trueValVariant = mMap.get( true );
  EXPECT_EQ( std::get<ManagedValue<MValueTypeI>>( trueValVariant ).getInstance().get(), 42 );
}

TEST(ManagedMap, SetAndGetFailure) {
  MMap mMap;
  //invalid add je compile-time error nebo warning
  mMap.add<MValueTypeI>( true );
  mMap.get<MValueTypeI>( true ).getInstance().init( 42.0 );
  //opakovanej add se stejnym klicem je run-time error
  EXPECT_THROW( mMap.add<MValueTypeI>( true ), std::logic_error );
  //fetch na nevlozenou hodnotu je run-time exception
  EXPECT_THROW( mMap.get<MValueTypeI>( false ).getInstance().get(), std::out_of_range );
  //fetch na v ManagedValueType... neobsazeny typ je compile-time error
  //mMap.get<MValueTypeF>( 1.2 );//this doesn't compile
}

//operator<=:
//- nad dvema stejnyma mapama
TEST(ManagedMap, OperatorLEWorksForSameMaps) {
  MMap mMap1, mMap2;
  mMap1.add<MValueTypeI>( true );
  mMap2.add<MValueTypeI>( true );
  mMap1.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap2.get<MValueTypeI>( true ).getInstance().init( 42 );
  EXPECT_LE( mMap1, mMap2 );
  EXPECT_LE( mMap2, mMap1 );
  mMap1.add<MValueTypeB>( false );
  mMap2.add<MValueTypeB>( false );
  mMap1.get<MValueTypeB>( false ).getInstance().init( false );
  mMap2.get<MValueTypeB>( false ).getInstance().init( false );
  EXPECT_LE( mMap1, mMap2 );
  EXPECT_LE( mMap2, mMap1 );
  mMap1.get<MValueTypeI>( true ).getInstance().set( 44 );
  mMap2.get<MValueTypeI>( true ).getInstance().set( 44 );
  EXPECT_LE( mMap1, mMap2 );
  EXPECT_LE( mMap2, mMap1 );
}

//- jedna mapa ma eliminovanejsi moznosti
TEST(ManagedMap, OperatorLEWorksForDifferentMaps) {
  MMap mMap1, mMap2;
  mMap1.add<MValueTypeI>( true );
  mMap2.add<MValueTypeI>( true );
  mMap1.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap2.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap1.get<MValueTypeI>( true ).getInstance().set( 44 );
  EXPECT_LE( mMap1, mMap2 );
  EXPECT_FALSE( mMap2 <= mMap1 );
}

//- kdyz nejaky klic neni v jedne mape pritomen
TEST(ManagedMap, OperatorLEWorksWithMissingKeyInMap2) {
  MMap mMap1, mMap2;
  mMap1.add<MValueTypeI>( true );
  mMap2.add<MValueTypeI>( true );
  mMap1.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap2.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap2.add<MValueTypeB>( false );
  mMap2.get<MValueTypeB>( false ).getInstance().init( true );
  EXPECT_LE( mMap1, mMap2 );
  EXPECT_FALSE( mMap2 <= mMap1 );
}

//- kdyz mapy obsahuji pod stejnym klicem ruzne Varianty
TEST(ManagedMap, OperatorLEWorksWithDifferentVariantsUnderSameKey) {
  MMap mMap1, mMap2;
  mMap1.add<MValueTypeI>( true );
  mMap1.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap2.add<MValueTypeB>( true );
  mMap2.get<MValueTypeB>( true ).getInstance().init( false );
  EXPECT_FALSE( mMap1 <= mMap2 );
  EXPECT_FALSE( mMap2 <= mMap1 );
}

//- kdyz mapy obsahuji neporovnatelne Varianty
TEST(ManagedMap, OperatorLEWorksWithIncomparableVariants) {
  MMap mMap1, mMap2;
  mMap1.add<MValueTypeI>( true );
  mMap1.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap1.get<MValueTypeI>( true ).getInstance().erase( 42 );
  mMap2.add<MValueTypeI>( true );
  mMap2.get<MValueTypeI>( true ).getInstance().init( 42 );
  mMap2.get<MValueTypeI>( true ).getInstance().erase( 43 );
  EXPECT_FALSE( mMap1 <= mMap2 );
  EXPECT_FALSE( mMap2 <= mMap1 );
  mMap1.get<MValueTypeI>( true ).getInstance().set( 43 );
  mMap2.get<MValueTypeI>( true ).getInstance().set( 42 );
  EXPECT_FALSE( mMap1 <= mMap2 );
  EXPECT_FALSE( mMap2 <= mMap1 );
}

}
