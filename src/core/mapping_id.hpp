#pragma once
#include<string>

namespace logicker::core {

class MappingIdBase {};

template<auto Id, typename K, typename V>
class MappingId : MappingIdBase {
  public:
    using KeyT = K;
    using ValueT = V;
};

}
