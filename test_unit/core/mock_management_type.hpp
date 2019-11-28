#pragma once
#include "gmock/gmock.h"
#include <bitset>
#include <memory>
#include <optional>

namespace {

template<typename Value_ = int>
class MockReadOnlyManagementType {
  private:
    class Mock {
      public:
        MOCK_CONST_METHOD0( mockedGet, void() );
        MOCK_CONST_METHOD0( mockedGetValueSet, void() );
    };
  public:
    void init( const Value_& value_ ) { value = value_; }

    const Value_& get() const {
      mock->mockedGet();
      if (! value ) throw std::logic_error{ "" };
      return *value;
    }

    std::set<Value_> getValueSet() const { mock->mockedGetValueSet(); return { get() }; }

    Mock& getMock() const { return *mock; }
  private:
    std::optional<Value_> value{};
    std::shared_ptr<Mock> mock = std::make_shared<Mock>();
};

//z tohohle udelam template templatu pomoci; melo by to fungovat
//class MockRangeManagement<Size_> {
//  template<Value_ = int>
//  using type = MockRangeManagementType<Size_, Value_>;
//};
template<std::size_t Size_, typename Value_ = int>
class MockRangeManagementType {
  private:
    class Mock {
      public:
        MOCK_METHOD1_T( mockedErase, void( Value_ ) );
        MOCK_METHOD1_T( mockedSet, void( Value_ ) );
        MOCK_CONST_METHOD0_T( mockedGet, void() );
        MOCK_CONST_METHOD0_T( mockedGetValueSet, void() );
    };
  public:
    static_assert( Size_ > 1);
    void init( const Value_& min_ ) { min = min_; }

    void erase( const Value_& val );
    void set( const Value_& val );
    const Value_& get() const;
    const std::set<Value_> getValueSet() const;

    Mock& getMock() const { return *mock; }
  private:
    std::optional<Value_> min{};
    std::bitset<Size_> erased{};
    std::optional<Value_> value{};
    std::shared_ptr<Mock> mock = std::make_shared<Mock>();
};

template<std::size_t Size_, typename Value_>
void
MockRangeManagementType<Size_, Value_>::erase( const Value_& val ) {
  mock->mockedErase( val );
  if ( ! min ) throw std::logic_error{ "init has not been called" };
  if ( value ) throw std::logic_error{ "final value has already been set" };
  int valIndex = val - *min;
  if ( erased.test( valIndex ) ) throw std::logic_error{ "value has already been erased" };
  erased.flip( valIndex );
  if ( erased.count() == Size_ - 1 ) {
    int indexToSet = 0;
    for ( ; erased.test( indexToSet ); ++indexToSet );
    value = *min + indexToSet;
  }
}

template<std::size_t Size_, typename Value_>
void
MockRangeManagementType<Size_, Value_>::set( const Value_& val ) {
  mock->mockedSet( val );
  if ( ! min ) throw std::logic_error{ "init has not been called" };
  if ( value ) throw std::logic_error{ "final value has already been set" };
  int valIndex = val - *min;
  if ( erased.test( valIndex ) ) throw std::logic_error{ "value cannot be set; already erased" };
  value = val;
}

template<std::size_t Size_, typename Value_>
const Value_&
MockRangeManagementType<Size_, Value_>::get() const {
  mock->mockedGet();
  if ( ! min ) throw std::logic_error{ "init has not been called" };
  if ( value ) {
    return *value;
  } else {
    throw std::logic_error{ "cannot get; value has not been set" };
  }
}

template<std::size_t Size_, typename Value_>
const std::set<Value_>
MockRangeManagementType<Size_, Value_>::getValueSet() const {
  mock->mockedGetValueSet();
  if ( ! min ) throw std::logic_error{ "init has not been called" };
  if ( value ) {
    return { *value };
  } else {
    std::set<Value_> result;
    for ( std::size_t i = 0; i < Size_; ++i ) {
      if ( ! erased.test( i ) ) {
        result.insert( *min + i );
      }
    }
    return result;
  }
}

template<std::size_t S>
class MockRangeManagementTemplate {
  public:
    template<typename T>
    using type = MockRangeManagementType<S, T>;
};

}
