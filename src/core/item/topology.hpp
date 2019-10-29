#pragma once

namespace logicker::core::item {

namespace topology {

  class RegionId {};

  class Region {};

  inline bool operator==( const Region& /*lhs*/, const Region& /*rhs*/ ) {
    return true;
  }

  inline bool operator!=( const Region& lhs, const Region& rhs ) {
    return ! ( lhs == rhs );
  }

}

class Topology {};

}
