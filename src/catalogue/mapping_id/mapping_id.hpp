#pragma once
#include "core/grid/mapping_id.hpp"
#include "core/item/collection/set.hpp"

namespace logicker::core::item {
  class Coords;
  class Topology {};
  class Square : public Topology {};
  class Region {};

  inline bool operator==( const Region& /*lhs*/, const Region& /*rhs*/ ) {
    return true;
  }

  inline bool operator!=( const Region& lhs, const Region& rhs ) {
    return ! ( lhs == rhs );
  }
}

namespace logicker::catalogue::mapping_id {
char availableValuesStr[] = "available_values";
char centersStr[] = "centers";
char regionsStr[] = "regions";
char sizeStr[] = "size";
char topologyStr[] = "topology";

class Size : core::MappingId<sizeStr, void, int> {};
class Topology : core::MappingId<topologyStr, void, core::item::Topology> {};
class Square : core::MappingId<topologyStr, void, core::item::Square> {};
template<typename ValueT>
class Centers : core::MappingId<centersStr, core::item::Coords, ValueT> {};
template<typename ValueT>
class AvailableValues : core::MappingId<availableValuesStr, void, core::item::collection::Set<ValueT>> {};

class Regions : core::MappingId<regionsStr, void, core::item::collection::Set<core::item::Region>> {};

}
