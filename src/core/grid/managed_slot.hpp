#pragma once

#include "core/grid/managed_type.hpp"
#include "core/grid/managed_value.hpp"

namespace logicker::core::grid {

struct ManagedSlot{};

template<auto name_, typename valueType_, typename managementType_>
struct ManagedValueSlotImpl : ManagedSlot {
  public:
    static constexpr auto name = name_;
    using valueType = valueType_;
    using managementType = managementType_;
    using managedType = ManagedValueType<valueType_, managementType_>;
};

template<typename valueId_, typename valueType_>
struct ManagedValueSlotFromId {
  public:
    static_assert(std::is_same_v<typename valueId_::type, typename valueType_::valueType>);
    using type = ManagedValueSlotImpl<valueId_::name, typename valueId_::type, typename valueType_::managementType>;
};

template<auto name_, typename valueType_>
struct ManagedValueSlotFromName {
  public:
    using type = ManagedValueSlotImpl<name_, typename valueType_::valueType, typename valueType_::managementType>;
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct ManagedMapSlotImpl : ManagedSlot {
  public:
    static constexpr auto name = name_;
    using keyType = keyType_;
    using valueTypesTuple = std::tuple<valueTypes_...>;
    using valueTypesSet = type::SetT<valueTypesTuple>;
    using managedType = ManagedMapType<keyType_, valueTypes_...>;
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct ManagedMapSlotImpl<name_, keyType_, std::tuple<valueTypes_...>> : ManagedSlot {
  public:
    static constexpr auto name = name_;
    using keyType = keyType_;
    using valueTypesTuple = std::tuple<valueTypes_...>;
    using valueTypesSet = type::SetT<valueTypesTuple>;
    using managedType = ManagedMapType<keyType_, valueTypes_...>;
};

//tahle sablona ma umoznovat namapovat mapId_ na libovolny mapType_ se stejnymi typy hodnot
//(a taky to umoznuje, byt s nedokonalym assertem na valueTypesSety nize)
//TODO: mela by existovat taky moznost definovat ManagedMapSlot pomoci mapId_ a managementType_,
//ktera by zadany managementType_ aplikovala na vsechny valueIds
template<typename mapId_, typename mapType_>
struct ManagedMapSlotFromId {
  public:
    static_assert(std::is_same_v<typename mapId_::keyType, typename mapType_::keyType>);
    //TODO: nasledujici assert je spatne: prvni typ je tuple of typenames, druhy typ je tuple
    //of ManagedValueTypes; proto assert neprochazi, kde by mel, a proto je zakomentovany
    //coz btw ukazuje na zajimavou vec, ktera me zatim nenapadla, a to je moznost mit
    //pro jeden typ ve smyslu typename nekolik ManagedValueTypes, lisicich se zpusobem
    //managementu: chci to povolit? nechci to povolit? jak to ovlivnuje API?
    //chci to povolit, API to neovlivni vubec. 
    //Ten nasledujici assert ma overit, ze kazdy typ z druheho tuple ma valueType, ktery
    //je obsazeny v prvnim tuplu. Tj. chce to specialni trait, ktery tuhle vlastnost overi
    //pro jeden typename, a druhy trait s dedukci parametr packu z mapType_::valueTypesSet,
    //ktery da hodnoty dohromady do jedineho vysledku
    //static_assert(type::is_same_set<typename mapId_::valueTypesSet, typename mapType_::valueTypesSet>);

    using type = ManagedMapSlotImpl<mapId_::name, typename mapId_::keyType, typename mapType_::valueTypesTuple>;
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct ManagedMapSlotFromName {
  public:
    using type = ManagedMapSlotImpl<name_, keyType_, std::tuple<valueTypes_...>>;
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct ManagedMapSlotFromName<name_, keyType_, std::tuple<valueTypes_...>> {
  public:
    using type = typename ManagedMapSlotFromName<name_, keyType_, valueTypes_...>::type;
};

template<typename managedId, template<typename> typename managementType>
struct MakeSlot;

template<auto name_, typename type_, template<typename> typename managementType_>
struct MakeSlot<ManagedValueId<name_, type_>, managementType_> {
  public:
    //using managementType = managementType_<type_>;
    //using managedValueType = ManagedValueType<type_, managementType>;
    //using value = typename ManagedValueSlotFromName<name_, managedValueType>::type;
    using value = typename ManagedValueSlotFromName<name_, ManagedValueType<type_, managementType_<type_>>>::type;
};

template<auto name_, typename keyType_, typename... valueTypes_, template<typename> typename managementType_>
struct MakeSlot<ManagedMapId<name_, keyType_, valueTypes_...>, managementType_> {
  public:
    using managedValues = std::tuple<ManagedValue<valueTypes_, managementType_<valueTypes_>>...>;
    using value = typename ManagedMapSlotFromName<name_, keyType_, managedValues>::type;
};

template<typename managedId, template<typename> typename managementType>
using makeSlot_v = typename MakeSlot<managedId, managementType>::value;

template<typename... managedSlots_>
struct ManagedSlotsSet {
  public:
    static_assert((std::is_base_of_v<ManagedSlot, managedSlots_> && ... ));
    using tuple = std::tuple<managedSlots_...>;
    using set = type::SetT<tuple>;
};

template<typename... managedSlots_>
using ManagedSlotsSetFromSlots = ManagedSlotsSet<managedSlots_...>;

template<typename managedIds, template<typename> typename managementType>
struct ManagedSlotsSetFromIdsImpl;

template<typename... managedIds, template<typename> typename managementType>
struct ManagedSlotsSetFromIdsImpl<std::tuple<managedIds...>, managementType> {
  public:
    using type = ManagedSlotsSet<makeSlot_v<managedIds, managementType>...>;
};

template<typename managedIdsSet, template<typename> typename managementType>
using ManagedSlotsSetFromIds = typename ManagedSlotsSetFromIdsImpl<typename managedIdsSet::tuple, managementType>::type;

}
