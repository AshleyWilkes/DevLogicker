#include "gtest/gtest.h"
#include "core/convertor.hpp"

namespace {

using namespace logicker::core::convertor;

template<typename T>
class input {
  public:
    input( const T& value ) : value_{ value } {}

    template<typename S>
    S get() const {
      if constexpr ( std::is_same_v<T, bool> && std::is_same_v<S, int> ) {
        throw std::invalid_argument( "" );
      } else {
        return value_;
      }
    }
  private:
    T value_;
};

//Convertor<T, T> vraci stejnou hodnotu, i kdyz na ni nelze zavolat get<T>
TEST(Convertor, OneArgTupleIdentity) {
  Convertor<std::tuple<int>> conv( std::make_tuple<>( 34 ) );
  EXPECT_EQ( conv.is_success(), true );
  EXPECT_EQ( conv.get(), std::make_tuple<>( 34 ) );
}

//Convertor<A, B> vraci success, kdyz projde metoda get<A>
TEST(Convertor, OneArgTupleSuccess) {
  Convertor<std::tuple<int>> conv( std::make_tuple<>( input<int>{ 34 } ) );
  EXPECT_EQ( conv.is_success(), true );
  EXPECT_EQ( conv.get(), std::make_tuple<>( 34 ) );
}

//Convertor<A, B> vraci failure, kdyz neprojde metoda get<A>
TEST(Convertor, OneArgTupleFailure) {
  Convertor<std::tuple<int>> conv( std::make_tuple<>( input<bool>{ true } ) );
  EXPECT_EQ( conv.is_success(), false );
  EXPECT_THROW( conv.get(), std::domain_error );
}

//Convertor<A, B> je compile time error, pokud neexistuje B::get<A>()
TEST(Convertor, OneArgCompileError) {
  //Convertor<std::tuple<int>> conv( std::make_tuple<>( 3.14 ) );//this should't compile
}

//Convertor<input<int>, input<bool> => int, bool> je success
TEST(Convertor, MultiArgTupleSuccess) {
  Convertor<std::tuple<int, bool>> conv( std::make_tuple<>( input<int>{ 42 }, input<bool>{ true } ) );
  EXPECT_EQ( conv.is_success(), true );
  EXPECT_EQ( conv.get(), std::make_tuple<>( 42, true ) );
}

//Convertor<input<int>, input<bool> => bool, int> je failure
TEST(Convertor, MultiArgTupleFailureDueToConversion) {
  Convertor<std::tuple<bool, int>> conv( std::make_tuple<>( input<int>{ 42 }, input<bool>{ true } ) );
  EXPECT_EQ( conv.is_success(), false );
  EXPECT_THROW( conv.get(), std::domain_error );
}

//Convertor<input<int>, input<bool> => int, bool, bool> je failure
TEST(Convertor, MultiArTuplegFailureDueToFormat) {
  Convertor<std::tuple<int, bool, bool>> conv( std::make_tuple<>( input<int>{ 42 }, input<bool>{ true } ) );
  EXPECT_EQ( conv.is_success(), false );
  EXPECT_THROW( conv.get(), std::domain_error );
}

}
