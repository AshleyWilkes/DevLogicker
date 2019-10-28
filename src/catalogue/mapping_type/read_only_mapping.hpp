#pragma once
#include "core/grid/mapping_id.hpp"

namespace logicker::mapping {

//??YAGNI: swap??
template<typename Id>
class ReadOnlyMapping {
  public:
    using KeyT = typename Id::KeyT;
    using ValueT = typename Id::ValueT;

    //KISS, std::map should be sufficient; the std::map can always be
    //constructed as an intermediary
    ReadOnlyMapping( const std::map<KeyT, ValueT>& contents ) : contents_{ contents } {}
    
    //return the value stored in the Mapping under the key or
    //throw std::out_of_range if the key is not present
    ValueT get( const KeyT& key ) const { return contents_.at( key ); }

    //return everything stored in the mapping as a std::map;
    //the map returned may contain copies of the actual mapping's contents
    std::map<KeyT, ValueT> asMap() const { return contents_; }
  private:
    std::map<KeyT, ValueT> contents_;
};

}
