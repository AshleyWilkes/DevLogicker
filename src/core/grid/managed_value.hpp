#pragma once

namespace logicker::core::grid {

template<typename valueType_, typename instanceType_>
class ManagedValueImpl {
  public:
    using ValueType = valueType_;
    using InstanceType = instanceType_;

    template<typename... Args>
    ManagedValueImpl( Args&&... args ) : instance{ std::forward<Args>( args )... } {}

    const InstanceType& getInstance() { return instance; }
  private:
    InstanceType instance;
};

template<typename... Args>
struct ManagedValueHelper;

template<typename managedValueType_>
struct ManagedValueHelper<managedValueType_> {
  public:
    using type = ManagedValueImpl<typename managedValueType_::valueType, typename managedValueType_::managementType>;
};

template<typename valueType_, typename managementType_>
struct ManagedValueHelper<valueType_, managementType_> {
  public:
    using type = ManagedValueImpl<valueType_, managementType_>;
};

template<typename... Args>
using ManagedValue = typename ManagedValueHelper<Args...>::type;

}
