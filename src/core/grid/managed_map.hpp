#pragma once
#include "core/grid/managed_value.hpp"
#include <map>
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
    using ValuesVariantType = std::variant<ManagedValue<valueTypes_>...>;

    template<typename valueType, typename... Args>
    void add( const KeyType& key, Args&&... args ) const;

    template<typename valueType>
    ManagedValue<valueType>& get( const KeyType& key );

    template<typename valueType>
    const ManagedValue<valueType>& get( const KeyType& key ) const;

    const ValuesVariantType get( const KeyType& key ) const;

    auto& getInstance() { return *this; } 
    const auto& getInstance() const { return *this; } 

    const std::map<KeyType, ValuesVariantType>& getMap() const { return map_; }
  private:
    mutable std::map<KeyType, ValuesVariantType> map_;
};

template<typename keyType_, typename... valueTypes_>
template<typename valueType, typename... Args>
void
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::add( const KeyType& key, Args&&... args ) const {
  if ( map_.find( key ) != map_.end() ) throw std::logic_error{ "duplicate key" };
  ManagedValue<valueType> value{ std::forward<Args>( args )... };
  ValuesVariantType valueVariant = std::move( value );
  map_.insert({ key, valueVariant });
}

template<typename keyType_, typename... valueTypes_>
template<typename valueType>
ManagedValue<valueType>&
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::get( const KeyType& key ) {
  return std::get<ManagedValue<valueType>>( map_.at( key ) );
}

template<typename keyType_, typename... valueTypes_>
template<typename valueType>
const ManagedValue<valueType>&
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::get( const KeyType& key ) const {
  return std::get<valueType>( map_.at( key ) );//.getInstance();
}

template<typename keyType_, typename... valueTypes_>
const typename ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::ValuesVariantType
ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>::get( const KeyType& key ) const {
  return map_.at( key );
}

template<typename variantType, typename lhType, typename rhType>
bool
compareVariants( const lhType& lhs, const rhType& rhs ) {
  if ( std::holds_alternative<ManagedValue<variantType>>( lhs ) ) {
    //vysledek lhVariant <= rhVariant, pokud obe obsahuji aktualni variantu
    if ( std::holds_alternative<ManagedValue<variantType>>( rhs ) ) {
      auto lhVariant = std::get<ManagedValue<variantType>>( lhs );
      auto rhVariant = std::get<ManagedValue<variantType>>( rhs );
      return lhVariant <= rhVariant;
    //false, pokud lhVariant obsahuje aktualni variantu, ale rhVariant ne
    } else  {
      return false;
    }
  } else {
    //true, pokud lhVariant obsahuje jinou variantu nez aktualni
    return true;
  }
}

template<typename fVariantType, typename... variantTypes>
class CompareVariants {
  public:
    template<typename lhType, typename rhType>
    static bool value( const lhType& lhs, const rhType& rhs ) {
      if ( compareVariants<fVariantType>( lhs, rhs ) ) {
        if constexpr ( sizeof...(variantTypes) > 0 ) {
          return CompareVariants<variantTypes...>::value( lhs, rhs );
        } else {
          return true;
        }
      } else {
        return false;
      }
    }
};

//plati map1 <= map2, prave kdyz:
//- pro kazdej key z map1 plati
//  - tento key je pritomen i v map2
//  - pod timto klicem je ve Variantu MValue stejnyho datovyho typu
//  - MValue v map1 <= MValue v map2
template<typename keyType1_, typename... valueTypes1_, typename keyType2_, typename... valueTypes2_>
bool
operator<=(
    const ManagedMapImpl<keyType1_, std::tuple<valueTypes1_...>>& lhs,
    const ManagedMapImpl<keyType2_, std::tuple<valueTypes2_...>>& rhs ) {
  for ( auto& lhValue : lhs.getMap() ) {
    auto key = lhValue.first;
    auto rhValue = rhs.getMap().find( key );
    if ( rhValue == rhs.getMap().end() ) { return false; }
    auto lhVariant = lhValue.second;
    auto rhVariant = rhValue->second;
    bool variantsComparisonResult = CompareVariants<valueTypes1_...>::value( lhVariant, rhVariant );
    if ( ! variantsComparisonResult ) {
      return false;
    }
  }
  return true;
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

