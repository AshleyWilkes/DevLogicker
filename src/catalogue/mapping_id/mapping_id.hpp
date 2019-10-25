#pragma once
#include "core/mapping_id.hpp"

namespace logicker::core::item {
  class Coords;
  class Topology;
}

namespace logicker::catalogue::mapping_id {
char centersStr[] = "centers";
char sizeStr[] = "size";
char topologyStr[] = "topology";

class Size : core::MappingId<sizeStr, void, int> {};
class Topology : core::MappingId<topologyStr, void, core::item::Topology> {};
template<typename ValueT>
class Centers : core::MappingId<centersStr, core::item::Coords, ValueT> {};

}
