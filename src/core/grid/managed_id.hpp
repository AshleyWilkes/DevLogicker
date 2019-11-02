#pragma once

#include "types_util/types_util.hpp"

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

}
