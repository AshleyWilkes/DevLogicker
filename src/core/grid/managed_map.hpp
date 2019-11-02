#pragma once
#include <variant>

namespace logicker::core::grid {

template<typename T>
struct Print;

template<typename keyType_, typename valueTypes_>
class ManagedMapImpl;

template<typename keyType_, typename... valueTypes_>
class ManagedMapImpl<keyType_, std::tuple<valueTypes_...>> {
  public:
    using KeyType = keyType_;
    using ValuesVariantType = std::variant<typename valueTypes_::managementType...>;

    template<typename valueType, typename... Args>
    void add( const KeyType& key, Args&&... args );

    template<typename valueType>
    const typename valueType::managementType& get( const KeyType& key );

    const ValuesVariantType get( const KeyType& key );
  private:
    std::map<KeyType, ValuesVariantType> map_;
};

template<typename keyType_, typename... valueTypes_>
template<typename valueType, typename... Args>
void
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::add( const KeyType& key, Args&&... args ) {
  if ( map_.find( key ) != map_.end() ) throw std::logic_error{ "duplicate key" };
  typename valueType::managementType value{ std::forward<Args>( args )... };
  ValuesVariantType valueVariant = value;
  map_.insert({ key, valueVariant });
}

template<typename keyType_, typename... valueTypes_>
template<typename valueType>
const typename valueType::managementType&
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::get( const KeyType& key ) {
  return std::get<typename valueType::managementType>( map_.at( key ) );
}

template<typename keyType_, typename... valueTypes_>
const typename ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::ValuesVariantType
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::get( const KeyType& key ) {
  return map_.at( key );
}

template<typename ManagedMapType>
using ManagedMap = ManagedMapImpl<typename ManagedMapType::keyType, typename ManagedMapType::valueTypesTuple>;

}
//instance ManagedMapTypu, tj. dvojice [typKlice, ManagedValueType...]
//interne je to tedy std::map<typeKlice, std::variant<ManagedValueType...>>
//
//z toho plyne, ze setter musi znat typ ManagedValueType, ktery ma byt pouzit;
//  v pripade, ze pro dany typ klice podporuju jen jednu moznost, lze pouzit tu
//  jedinou moznost, jinak je nutno explicitne uvest. Implementace? hm.
//Setter forwarduje sve argumenty do konstruktoru prislusneho ManagedValue
//
//oproti tomu getter je genericky, resp. vraci std::variant a volajici uz si s tim poradi
//nicmene budu na volajiciho hodnej, beztak jsem to ja, a dam mu k dispozici i
//  negenerickej na vysledek templatovanej getter, kterej vrati primo spravny
//  ManagementType<typHodnoty> nebo vyjimku
//

