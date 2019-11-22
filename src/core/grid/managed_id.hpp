#pragma once

#include "types_util/types_util.hpp"
#include <string_view>

namespace logicker::core::grid {

struct ManagedId{};

template<auto name_, typename type_>
struct ManagedValueId : public ManagedId {
  public:
    static constexpr auto name = name_;
    using type = type_;
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct ManagedMapId : public ManagedId {
  public:
    static constexpr auto name = name_;
    using keyType = keyType_;
    using valueTypesTuple = std::tuple<valueTypes_...>;
    using valueTypesSet = type::SetT<valueTypesTuple>;
};

template<typename... managedIds_>
struct ManagedIdsSet {
  public:
    static_assert((std::is_base_of_v<ManagedId, managedIds_> && ...));
    using tuple = std::tuple<managedIds_...>;
    using set = type::SetT<tuple>;
};

//ManagedIds jsou porovnatelne na inkluzi (<=); operaci splnuji:
//dve stejna ManagedValueIds 
//dve ManagedMapIds, kde name_ a keyType_ jsou stejne a valueTypes_ prvniho je (neostra) podmnozina valueTypes_ druheho
//vsechny ostatni kombinace dvou ManagedIds operaci nesplnuji.

template<typename MId1, typename MId2>
class IsSubMId {
  public:
    static inline constexpr bool value = false;
};

template<auto name1, typename type1, auto name2, typename type2>
class IsSubMId<ManagedValueId<name1, type1>, ManagedValueId<name2, type2>> {
  public:
    static inline constexpr bool value = 
      //std::string_view( name1 ) == name2 &&
      std::is_same_v<type1, type2>;
};

template<auto name1, typename key1, typename... values1, auto name2, typename key2, typename... values2>
class IsSubMId<ManagedMapId<name1, key1, values1...>, ManagedMapId<name2, key2, values2...>> {
  public:
    static inline constexpr bool value = 
      //std::string_view( name1 ) == name2 &&
      std::is_same_v<key1, key2> &&
      type::is_subset<type::SetT<std::tuple<values1...>>, type::SetT<std::tuple<values2...>>>;
};

template<typename MId1, typename MId2>
inline constexpr bool is_sub_MId_v = IsSubMId<MId1, MId2>::value;

}
