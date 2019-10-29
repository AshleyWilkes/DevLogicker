#pragma once
#include "core/grid/mapping_id.hpp"
#include "core/item/collection/set.hpp"
#include "core/item/topology/rectangle.hpp"
#include "core/item/topology/square.hpp"

namespace logicker::core::item {
  class Coords;
}

namespace logicker::catalogue::mapping_id {
char availableValuesStr[] = "availableValues";
char centersStr[] = "centers";
char heightStr[] = "height";
char regionsStr[] = "regions";
char regionValuesStr[] = "regionValues";
char sizeStr[] = "size";
char topologyStr[] = "topology";
char uDirectionCluesStr[] = "uDirectionClues";
char widthStr[] = "width";

template<typename ValueT>
class AvailableValues : core::MappingId<availableValuesStr, void, core::item::collection::Set<ValueT>> {};
template<typename ValueT>
class Centers : core::MappingId<centersStr, core::item::Coords, ValueT> {};
class Height : core::MappingId<heightStr, void, int> {};
class Rectangle : core::MappingId<topologyStr, void, core::item::topology::Rectangle> {};
class Regions : core::MappingId<regionsStr, core::item::topology::RegionId, core::item::topology::Region> {};
template<typename ValueT>
class RegionValues : core::MappingId<regionValuesStr, core::item::topology::RegionId, ValueT> {};
template<auto id, typename Type>
class Scalar : core::MappingId<id, void, Type> {};
class Size : core::MappingId<sizeStr, void, int> {};
class Square : core::MappingId<topologyStr, void, core::item::topology::Square> {};
class Topology : core::MappingId<topologyStr, void, core::item::Topology> {};
template<typename ClueType>
class UDirectionClues : core::MappingId<uDirectionCluesStr, core::item::topology::RegionId, ClueType> {};
class Width : core::MappingId<widthStr, void, int> {};

}
