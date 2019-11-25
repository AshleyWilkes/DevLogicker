#pragma once

namespace logicker::core::grid {

template<typename valueType_, typename instanceType_>
class ManagedValueImpl {
  public:
    using ValueType = valueType_;
    using InstanceType = instanceType_;
    using managementType = instanceType_;

    template<typename... Args>
    ManagedValueImpl( Args&&... args ) : instance{ std::forward<Args>( args )... } {}

    InstanceType& getInstance() { return instance; }
    const InstanceType& getInstance() const { return instance; }
  private:
    InstanceType instance;
};

template<typename valueType1_, typename instanceType1_, typename valueType2_, typename instanceType2_>
bool
operator<=(
    const ManagedValueImpl<valueType1_, instanceType1_>& lhs,
    const ManagedValueImpl<valueType2_, instanceType2_>& rhs ) {
  auto lhSet = lhs.getInstance().getValueSet();
  auto rhSet = rhs.getInstance().getValueSet();
  auto lhIt = lhSet.begin();
  auto rhIt = rhSet.begin();
  //chci, aby vsechno, co je v lhSet, bylo i v rhSet; iteratory obou setu jsou setridene
  while ( lhIt != lhSet.end() && rhIt != rhSet.end() ) {
    if ( *lhIt == *rhIt ) {
      ++lhIt;
    } else if ( *lhIt > *rhIt ) {
      ++rhIt;
    } else {
      return false;
    }
  }
  return lhIt == lhSet.end();
}

template<typename... Args>
struct ManagedValueHelper;

template<typename valueType_, typename managementType_>
struct ManagedValueHelper<ManagedValueImpl<valueType_, managementType_>> {
  public:
    using type = ManagedValueImpl<valueType_, managementType_>;
};

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
