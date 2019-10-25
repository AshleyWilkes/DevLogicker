#include "gtest/gtest.h"
#include "core/grid.hpp.old"
#include "catalogue/mapping_type/read_only_mapping.hpp"

namespace {

char mappingStr1[] = "int2int mapping";
using MappingId1 = logicker::core::MappingId<mappingStr1, int, int>;
char mappingStr2[] = "str2str mapping";
using MappingId2 = logicker::core::MappingId<mappingStr2, std::string, std::string>;

template<typename T>
using MappingType = logicker::mapping::ReadOnlyMapping<T>;

using Grid = logicker::core::Grid<MappingType, MappingId1, MappingId2>;

class GridFixture : public testing::Test {
  public:
    GridFixture() {
      grid_.set<MappingId1>( int_values_ );
      grid_.set<MappingId2>( str_values_ );
    }
  protected:
    Grid grid_;
    std::map<int, int> int_values_{{1,1},{2,3}};
    std::map<std::string, std::string> str_values_{{"one", "first"},{"two", "second"}};
};

//test: k hodnotam v gridu lze pristouit
TEST_F(GridFixture, Construct) {
  EXPECT_EQ( grid_.get<MappingId1>().asMap(), int_values_ );
  EXPECT_EQ( grid_.get<MappingId2>().asMap(), str_values_ );
}

//test: grid jde zkopirovat a lze pristoupit k hodnotam v kopii
TEST_F(GridFixture, CopyGrid) {
  Grid copied{ grid_ };
  //test the copy
  EXPECT_EQ( copied.get<MappingId1>().asMap(), int_values_ );
  EXPECT_EQ( copied.get<MappingId2>().asMap(), str_values_ );
  //test the original
  EXPECT_EQ( grid_.get<MappingId1>().asMap(), int_values_ );
  EXPECT_EQ( grid_.get<MappingId2>().asMap(), str_values_ );
}

//test: grid jde zmovovat a  lze pristoupit k hodnotam v kopii (a v originale ne)
TEST_F(GridFixture, MoveGrid) {
  Grid moved{ std::move( grid_ ) };
  //test the copy
  EXPECT_EQ( moved.get<MappingId1>().asMap(), int_values_ );
  EXPECT_EQ( moved.get<MappingId2>().asMap(), str_values_ );
  //test the original
  EXPECT_THROW( grid_.get<MappingId1>(), std::domain_error );
  EXPECT_THROW( grid_.get<MappingId2>(), std::domain_error );
}

}
