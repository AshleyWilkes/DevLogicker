#include "gtest/gtest.h"
#include "catalogue/mapping_type/read_only_mapping.hpp"

namespace {

char testMappingStr[] = "testMapping";
using MappingId = logicker::core::MappingId<testMappingStr, int, int>;
using MappingT = logicker::mapping::ReadOnlyMapping<MappingId>;

//fixture for the tests
class ReadOnlyMappingFixture : public testing::Test {
  protected:
    std::map<int, int> values_{{1,1}, {2,2}};
    MappingT mapping_{ values_ };
};

//-- successfully create a mapping
TEST_F(ReadOnlyMappingFixture, Construct) {
  EXPECT_EQ( mapping_.asMap(), values_ );
}

//-- succesfully extract existing value
TEST_F(ReadOnlyMappingFixture, GetExistingValue) {
  EXPECT_EQ( mapping_.get( 1 ), 1 );
  EXPECT_EQ( mapping_.get( 2 ), 2 );
}

//-- get exception when extracting non-existing value
TEST_F(ReadOnlyMappingFixture, GetNonExistingValue) {
  EXPECT_THROW( mapping_.get( 0 ), std::out_of_range );
}

//-- use a copy-construction made copy
TEST_F(ReadOnlyMappingFixture, CopyMapping) {
  MappingT copied{ mapping_ };
  //test the copy
  EXPECT_EQ( copied.get( 1 ), 1 );
  EXPECT_EQ( copied.get( 2 ), 2 );
  EXPECT_THROW( copied.get( 0 ), std::out_of_range );
  //test the original
  EXPECT_EQ( mapping_.get( 1 ), 1 );
  EXPECT_EQ( mapping_.get( 2 ), 2 );
  EXPECT_THROW( mapping_.get( 0 ), std::out_of_range );
}

//-- use a move-construction made copy
TEST_F(ReadOnlyMappingFixture, MoveMapping) {
  MappingT moved{ std::move( mapping_)  };
  //test the copy
  EXPECT_EQ( moved.get( 1 ), 1 );
  EXPECT_EQ( moved.get( 2 ), 2 );
  EXPECT_THROW( moved.get( 0 ), std::out_of_range );
  //test the original
  EXPECT_THROW( mapping_.get( 0 ), std::out_of_range );
  EXPECT_THROW( mapping_.get( 1 ), std::out_of_range );
  EXPECT_THROW( mapping_.get( 2 ), std::out_of_range );
}

}
