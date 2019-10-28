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

}
