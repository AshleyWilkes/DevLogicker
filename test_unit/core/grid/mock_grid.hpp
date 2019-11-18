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

template<typename RealGrid = void>
class MockGrid {
  public:
    MockGrid( RealGrid& grid ) : grid_{ grid } {}

    MOCK_METHOD0( mockedGet, void() );

    template<typename MId>
    auto& get() { mockedGet(); return grid_.template get<MId>(); }
  private:
    RealGrid& grid_;
};

template<>
class MockGrid<void> {
  public:
    class DummyManagedValue{};

    MOCK_METHOD0( mockedGet, void() );

    template<typename MId>
    auto& get() { mockedGet(); return instance_; }
  private:
    DummyManagedValue instance_;
};
