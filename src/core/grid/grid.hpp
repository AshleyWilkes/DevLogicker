#pragma once

//chtelo by to mit jmena pro nekolik stupnu na ceste od MappingId k full
//instantiated Gridu:
//
//MappingIdSet -- mnozina MappingId
//Mapping -- dvojice <MappingType, MappingId>
//GridType -- mnozina Mappingu: 
//    muze vzniknout jako Set Mappingu anebo jako dvojice <MappingType, MappingIdSet>
//Grid -- objekt, ktery opravdu ma vsechny instance mappingu popsane v mnozine GridType
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
