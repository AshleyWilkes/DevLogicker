#pragma once

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
