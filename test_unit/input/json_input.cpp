#include "gtest/gtest.h"
#include "input/json_input.hpp"
#include "boost/dynamic_bitset.hpp"

namespace {

using namespace logicker::core::input;

//1) fctionality needed to fulfill the input interface
//1a) invalid_argument is thrown upon request to non-existing name
TEST(InputTest, InvalidArgument) {
  //arrange
  json_input input = build_input( "{}" );
  //act
  EXPECT_THROW( input.get<int>( "name" ), std::invalid_argument );
}

//1b) bool value is correctly returned
TEST(InputTest, BoolValue) {
  //arrange
  json_input input = build_input( "{ \"name\": true }" );
  //act
  bool value = input.get<bool>( "name" );
  //assert
  EXPECT_EQ( value, true );
}

//1c) int values are correctly returned
TEST(InputTest, IntValue) {
  //arrange
  json_input input = build_input( "{"
      "\"norm\": 0,"
      "\"min\": -32768,"
      "\"max\": 32767 }" );
  //act
  int norm_value = input.get<int>( "norm" );
  int min_value = input.get<int>( "min" );
  int max_value = input.get<int>( "max" );
  //assert
  EXPECT_EQ( norm_value, 0);
  EXPECT_EQ( min_value, -32768);
  EXPECT_EQ( max_value, 32767);
}

//1d) string values are correctly returned
TEST(InputTest, StringValue) {
  //arrange
  std::string long_value_string( 65536, 'A' );
  json_input input = build_input( "{"
      "\"empty\": \"\","
      "\"non-empty\": \"ASQW\","
      "\"long\": \"" + long_value_string + "\" }" );
  //act
  std::string empty_str = input.get<std::string>( "empty" );
  std::string non_empty_str = input.get<std::string>( "non-empty" );
  std::string long_str = input.get<std::string>( "long" );
  //assert
  EXPECT_EQ( empty_str, "");
  EXPECT_EQ( non_empty_str, "ASQW");
  EXPECT_EQ( long_str, long_value_string );
}

//1e) vector of ints values are correctly returned
TEST(InputTest, VectorOfIntsValue) {
  //arrange
  std::string long_vector_string{ "1" };
  for ( int i = 2; i <= 65536; ++i ) {
    long_vector_string += ", " + std::to_string( i );
  }
  json_input input = build_input( "{"
      "\"empty\": [],"
      "\"non-empty\": [1, 2, 3, 4],"
      "\"long\": [ " + long_vector_string + " ]"
      "}" );
  //act
  std::vector<int> empty_vector = input.get<std::vector<int>>( "empty" );
  std::vector<int> non_empty_vector = input.get<std::vector<int>>( "non-empty" );
  std::vector<int> long_vector = input.get<std::vector<int>>( "long" );
  //assert
  EXPECT_EQ( empty_vector.size(), 0 );
  EXPECT_EQ( non_empty_vector.size(), 4 );
  for ( int i = 0; i < ( int ) non_empty_vector.size(); ++i ) {
    EXPECT_EQ( non_empty_vector.at( i ), i + 1 );
  }
  EXPECT_EQ( long_vector.size(), 65536 );
  for ( int i = 0; i < ( int ) long_vector.size(); ++i ) {
    EXPECT_EQ( long_vector.at( i ), i + 1);
  }
}

//1f) vector of bools values are correctly returned
TEST(InputTest, VectorOfBoolsValue) {
  //arrange
  json_input input = build_input( "{ \"name\": [ true, false ] }" );
  //act
  std::vector<bool> value = input.get<std::vector<bool>>( "name" );
  //assert
  EXPECT_EQ( value.size(), 2 );
  for ( int i = 0; i < ( int ) value.size(); ++i ) {
    EXPECT_EQ( value.at( i ), ( i == 0 ) );
  }
}

//1g) vector of strings values are correctly returned
TEST(InputTest, VectorOfStringsValue) {
  //arrange
  json_input input = build_input( "{ \"name\": [ \"alpha\", \"beta\", \"gam\" ] }" );
  //act
  std::vector<std::string> value = input.get<std::vector<std::string>>( "name" );
  //assert
  EXPECT_EQ( value.size(), 3 );
  for ( int i = 0; i < ( int ) value.size(); ++i ) {
    EXPECT_EQ( value.at( i ).size(), 5 - i );
  }
}

//1h) vector of vectors values are correctly returned
TEST(InputTest, VectorOfVectorsOfIntsValues) {
  //arrange
  json_input input = build_input( "{ \"name\": [ [ 1, 2 ], [ 3, 4] ] }" );
  //act
  std::vector<std::vector<int>> value = input.get<std::vector<std::vector<int>>>( "name" );
  //assert
  EXPECT_EQ( value.size(), 2 );
  for ( int i = 0; i < ( int ) value.size(); ++i ) {
    std::vector<int> element = value.at( i );
    EXPECT_EQ( value.size(), 2 );
    for ( int j = 0; j < ( int ) element.size(); ++j ) {
      EXPECT_EQ( element.at( j ), i * 2 + j + 1 );
    }
  }
}

template<typename T>
using vec4d = std::vector<std::vector<std::vector<std::vector<T>>>>;

template<typename T>
T vec4d_value( const vec4d<T>& vec, int i1, int i2, int i3, int i4 ) {
  return vec.at( i1 ).at( i2 ).at( i3 ).at( i4 );
}

//1i) 4d vector of ints values are correctly returned
TEST(InputTest, Vector4DIntsValues) {
  //arrange
  std::string lvl2_string = "[ [ 1, 2 ], [ 3, 4 ] ]";
  std::string lvl3_string = "[ " + lvl2_string + ", " + lvl2_string + " ]";
  std::string lvl4_string = "[ " + lvl3_string + ", " + lvl3_string + " ]";
  json_input input = build_input( "{ \"name\": " + lvl4_string + " }" );
  //act
  vec4d<int> value = input.get<vec4d<int>>( "name" );
  //assert
  EXPECT_EQ( vec4d_value( value, 0, 0, 0, 0 ), 1 );
  EXPECT_EQ( vec4d_value( value, 1, 1, 1, 1 ), 4 );
}

//1j) 4d vector of bools values are correctly returned
TEST(InputTest, Vector4DBoolsValues) {
  //arrange
  std::string lvl2_string = "[ [ true, false ], [ true, false ] ]";
  std::string lvl3_string = "[ " + lvl2_string + ", " + lvl2_string + " ]";
  std::string lvl4_string = "[ " + lvl3_string + ", " + lvl3_string + " ]";
  json_input input = build_input( "{ \"name\": " + lvl4_string + " }" );
  //act
  vec4d<bool> value = input.get<vec4d<bool>>( "name" );
  //assert
  EXPECT_EQ( vec4d_value( value, 0, 0, 0, 0 ), true );
  EXPECT_EQ( vec4d_value( value, 1, 1, 1, 1 ), false );
}

//1k) vector of bools values are correctly returned as bitsets
TEST(InputTest, VectorOfBoolsValuesAsBitsets) {
  //arrange
  std::string single_value_string{ "true" };
  std::string value_string{ single_value_string };
  for ( unsigned i = 1; i < 65536; ++i ) {
    value_string += ", " + single_value_string;
  }
  json_input input = build_input( "{"
      "\"empty\": [],"
      "\"non-empty\": [ true, false, true, false, true ],"
      "\"long\": [ " + value_string + " ] }" );
  //act
  boost::dynamic_bitset<> empty_bitset = input.get<boost::dynamic_bitset<>>( "empty" );
  boost::dynamic_bitset<> non_empty_bitset = input.get<boost::dynamic_bitset<>>( "non-empty" );
  boost::dynamic_bitset<> long_bitset = input.get<boost::dynamic_bitset<>>( "long" );
  //assert
  EXPECT_EQ( empty_bitset.size(), 0 );
  EXPECT_EQ( non_empty_bitset.size(), 5 );
  EXPECT_EQ( non_empty_bitset.count(), 3 );
  EXPECT_EQ( long_bitset.size(), 65536 );
}

//1l) 2d vector of bools values correctly returned as vector of bitsets
TEST(InputTest, Vector2DOfBoolsAsVectorOfBitsets) {
  //arrange
  json_input input = build_input( "{ \"name\": [ [ true, false ], [ true, false ] ] }" );
  //act
  using vec_bitset = std::vector<boost::dynamic_bitset<>>;
  vec_bitset value = input.get<vec_bitset>( "name" );
  //assert
  EXPECT_EQ( value.at( 0 ).size(), 2 );
  EXPECT_EQ( value.at( 1 ).count(), 1 );
}

//1m) recursive input
TEST(InputTest, RecursiveInput) {
  //arrange
  json_input input = build_input( "{ \"name\": { \"child_name\": 1 } }" );
  //act
  json_input child_input = input.get<json_input>( "name" );
  int child_int = child_input.get<int>( "child_name" );
  //assert
  EXPECT_EQ( child_int, 1 );
}

//2) json_input specific stuff
//2a) on incorrect input the json_input ctor throw an exception
//    std::invalid_argument describing the problem
TEST(JsonInputTest, InvalidInput) {
  EXPECT_THROW( 
      build_input( "ASD[}{" ),
      std::invalid_argument );
}

}
