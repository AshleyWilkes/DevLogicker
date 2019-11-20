#pragma once
#include "gmock/gmock.h"

template<typename RealInput = void>
class MockInput {
  public:
    MockInput( RealInput& input ) : input_{ input } {}

    MOCK_CONST_METHOD0( mockedGet, void() );
    MOCK_CONST_METHOD0( mockedGetInput, void() );

    template<typename T>
    const T get( const std::string& name ) const {
      if ( name == "input" ) mockedGetInput();
      mockedGet(); 
      return input_.template get<T>( name );
    }
  private:
    RealInput& input_;
};

template<>
class MockInput<void> {
  public:
    MOCK_CONST_METHOD0( mockedGet, void() );
    MOCK_CONST_METHOD0( mockedGetInput, void() );

    template<typename T>
    const T get( const std::string& name ) const {
      if ( name == "input" ) mockedGetInput();
      mockedGet();
      return T{};
    }
};
