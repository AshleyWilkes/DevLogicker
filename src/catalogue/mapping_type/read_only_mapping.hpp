#pragma once
#include "core/grid/mapping_id.hpp"

//odhalen novy, moderni pristup k MappingTypu:
//vsechny MappingTypy<K, V> jsou v dusledku mapy
//nejakeho typu <K, SomethingOf<V>>
//tedy vsechny jsou mapy a klice ve vsech jsou 
//primo typ K, takto klic Mappingu
//rozdil je jen v tom, jak pracuji s hodnotami
//typu V
//
//uz jmeno Mapping je asi divny
//mame Scalar, kterej neobsahuje zadnej klic, jenom V
//
//a SomethingOf<V> si taky zaslouzi poradnejsi nazev
//
//ok, takze nova generace pracuje s pojmem ManagedValue<V>
//Scalar je totez, co ManagedValue
//dale mame pojem ManagedMap<K, V>, coz je futral nad 
//std::map<K, ManagedValue<V>>
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
