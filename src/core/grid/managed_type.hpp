#pragma once

#include "types_util/types_util.hpp"

namespace logicker::core::grid {

struct ManagedType{};

template<typename valueType_, typename managementType_>
struct ManagedValueType : public ManagedType {
  public:
    using valueType = valueType_;
    using managementType = managementType_;
};

template<typename keyType_, typename... valueTypes_>
struct ManagedMapType : public ManagedType {
  public:
    static_assert((std::is_base_of_v<ManagedType, valueTypes_> && ...));
    using keyType = keyType_;
    using valueTypesTuple = std::tuple<valueTypes_...>;
    using valueTypesSet = type::SetT<valueTypesTuple>;
};

}
