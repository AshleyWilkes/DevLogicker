#pragma once
#include "gmock/gmock.h"

//sem chci dat MockGrid
//to ma bejt trida, obalujici nejaky existujici Grid, predany jako template argument,
//delegujici vsechna volani na instanci, kterou obaluje, a poskytujici navic vlastnosti
//google mocku, tj schopnost pridavat expectations apod.
//
//kdyz nema co obalit (parametr je void), tak nedela nic, jen poskytuje implementaci
//sveho interface; jelikoz musi neco vracet, ma jednu instanci zcela dummy typu, a tu vraci
//
//Grid ma jedinou metodu, template<typename T> get(), ktera vraci ManagedValue

template<typename RealManagedValue = void>
class MockManagedValue {
  public:
    MockManagedValue( RealManagedValue& value ) : value_{ value } {}

    MOCK_CONST_METHOD0( mockedGet, void() );
    MOCK_METHOD1( mockedSet, void( int ) );

    auto& get() { mockedGet(); return value_.get(); }
    const auto& get() const { mockedGet(); return value_.get(); }
    void set( int i ) { mockedSet( i ); value_.set( i ); }
  private:
    RealManagedValue& value_;
};

template<>
class MockManagedValue<void> {
  public:
    MOCK_CONST_METHOD0( mockedGet, void() );
    MOCK_METHOD1( mockedSet, void( int ) );

    auto& get() { mockedGet(); return instance_; }
    const auto& get() const { mockedGet(); return instance_; }
    void set( int i ) { mockedSet( i ); instance_ = i; }
  private:
    int instance_{ 8123 };
};


template<typename RealGrid = void>
class MockGrid {
  public:
    MockGrid( RealGrid& grid ) : grid_{ grid } {}

    MOCK_CONST_METHOD0( mockedGet, void() );

    template<typename MId>
    auto& get() { mockedGet(); return grid_.template get<MId>(); }

    template<typename MId>
    const auto& get() const { mockedGet(); return grid_.template get<MId>(); }
  private:
    RealGrid& grid_;
};

template<>
class MockGrid<void> {
  public:
    MOCK_CONST_METHOD0( mockedGet, void() );

    template<typename MId>
    auto& get() { mockedGet(); return instance_; }

    template<typename MId>
    const auto& get() const { mockedGet(); return instance_; }
  private:
    MockManagedValue<> instance_;
};
