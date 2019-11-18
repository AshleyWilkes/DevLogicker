#pragma once
#include "gmock/gmock.h"

//sem chci dat MockImplementationsRegistry
//to ma bejt trida, obalujici nejaky existujici ImplementationsRegistry, predany jako template argument,
//delegujici vsechna volani na instanci, kterou obaluje, a poskytujici navic vlastnosti
//google mocku, tj schopnost pridavat expectations apod.
//
//kdyz nema co obalit (parametr je void), tak nedela nic, jen poskytuje implementaci
//sveho interface; jelikoz musi neco vracet, ma jednu instanci zcela dummy typu, a tu vraci
//
//ImplementationsRegistry ma jedinou metodu, template<typename... Args> perform(), ktera vraci int

template<typename RealRegistry = void>
class MockImplementationsRegistry {
  public:
    MockImplementationsRegistry( RealRegistry& registry ) : registry_{ registry } {}

    MOCK_METHOD0( mockedPerform, void() );

    template<typename... Args>
    int perform( const Args&... args )
    { mockedPerform(); return registry_.template perform<Args...>( args... ); }
  private:
    RealRegistry& registry_;
};

template<>
class MockImplementationsRegistry<void> {
  public:
    MOCK_METHOD0( mockedPerform, void() );

    template<typename... Args>
    int perform( const Args&... ) { mockedPerform(); return 0; }
};
