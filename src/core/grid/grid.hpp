#pragma once

//chtelo by to mit jmena pro nekolik stupnu na ceste od ManagedValueId
//k full instantiated Gridu:
//
//ManagedValueId -- id, oznacujici 1 managed hodnotu: [jmeno, typHodnoty]
//ManagedMapId -- id, oznacujici mapu managed hodnot: [jmeno, typKlice, typHodnoty...]
//ManagedIdsSet -- mnozina, jejimiz prvky jsou ManagedValueIds a ManagedMapIds
//
//ManagementType -- zpusob, jak 1 hodnotu managuju; zda se, ze (na sebe kolme) moznosti jsou 
//    jednoduse store hodnoty VS domaina, z niz lze vyskrtavat
//    optional (muze hodnotu nemit) VS obligatory (musi mit hodnotu, jinak error)
//
//ManagedValueType -- dvojice [typHodnoty, ManagementType]
//ManagedMapType -- dvojice [typKlice, ManagedValueType...]
//
//ManagedValueSlot -- kompatibilni dvojice [ManagedValueId, ManagedValueType], 
//      jinak tez dvojice [jmeno, ManagedValueType],
//      jinak tez trojice [jmeno, typHodnoty, ManagementType]
//ManagedMapSlot -- kompatibilni dvojice [ManagedMapId, ManagedMapType],
//      jinak tez dvojice [jmeno, ManagedValueType...]
//
//ManagedSlotsSet -- mnozina, jejimiz prvky jsou ManagedValueSlots a ManagedMapSlots
//
//ManagedXXXType je instancializovatelny na ManagedXXX
//
//Grid je instancializovany ManagedSlotsSet v tom smyslu, ze kazdemu ManagedXXXSlotu je prirazena jedna instance ManagedXXX

namespace logicker::core {

template<template<typename...> typename MappingType, typename GridType>
class Grid {
  public:
    template<typename MappingId>
    MappingType<MappingId> get();

    template<typename MappingId>
    void set( const std::map<typename MappingId::KeyT, typename MappingId::ValueT>& values );
};

//pozadavky:
//rhs musi obsahovat vsechny MappingIds, ktere obsahuje lhs -- static_assert
//kazdy Mapping v lhs musi byt podmnozinou Mappingu v rhs -- volani porovnani na mappingach
template<template<typename...> typename MT1, typename GT1, template<typename...> typename MT2, typename GT2>
bool operator<=( const Grid<MT1, GT1>& lhs, const Grid MT2, GT2>& rhs ) {
  return false;
}

}
