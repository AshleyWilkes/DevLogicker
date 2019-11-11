#include "gtest/gtest.h"
#include "core/grid/managed_type.hpp"
#include "core/grid/managed_map.hpp"
#include "core/grid/common.hpp"

namespace {

using MValueTypeI = ManagedValueType<int, typename DummyManagementType::type<int>>;
using MValueTypeB = ManagedValueType<bool, typename DummyManagementType::type<bool>>;
using MValueTypeF = ManagedValueType<float, typename DummyManagementType::type<float>>;

using MMapType = ManagedMapType<bool, MValueTypeI, MValueTypeB>;

using MMap = ManagedMap<MMapType>;

//ManagedMap:
TEST(ManagedMap, SetAndGetSuccess) {
  //jde vytvorit, addnout hodnotu, getnout ji
  MMap mMap;
  mMap.add<MValueTypeI>( true, 42 );
  mMap.add<MValueTypeB>( false, false );
  EXPECT_EQ( mMap.get<MValueTypeI>( true ).get(), 42);
  EXPECT_EQ( mMap.get<MValueTypeB>( false ).get(), false );
  const auto& trueValVariant = mMap.get( true );
  EXPECT_EQ( std::get<typename MValueTypeI::managementType>( trueValVariant ).get(), 42 );
}

TEST(ManagedMap, SetAndGetFailure) {
  MMap mMap;
  //invalid add je compile-time error nebo warning
  mMap.add<MValueTypeI>( true, 42.0 );
  //opakovanej add se stejnym klicem je run-time error
  EXPECT_THROW( mMap.add<MValueTypeI>( true, 42 ), std::logic_error );
  //fetch na nevlozenou hodnotu je run-time exception
  EXPECT_THROW( mMap.get<MValueTypeI>( false ).get(), std::out_of_range );
  //fetch na v ManagedValueType... neobsazeny typ je compile-time error
  //mMap.get<MValueTypeF>( 1.2 );//this doesn't compile
}

}
