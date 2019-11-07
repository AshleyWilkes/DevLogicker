#include "gtest/gtest.h"
#include "core/grid/managed_id.hpp"
#include "core/grid/managed_type.hpp"
#include "core/grid/managed_slot.hpp"
#include "core/grid/common.hpp"

namespace {

char valueStr[] = "value";
using MValueId = ManagedValueId<valueStr, int>;

char mapStr[] = "map";
using MMapIdBIF = ManagedMapId<mapStr, bool, int, float>;

using MValueTypeI = ManagedValueType<int, DummyManagementType<int>>;
using MValueTypeF = ManagedValueType<float, DummyManagementType<float>>;

using MMapType = ManagedMapType<bool, MValueTypeI, MValueTypeF>;

using MValueSlot = typename ManagedValueSlotFromId<MValueId, MValueTypeI>::type;
using MValueSlot2 = typename ManagedValueSlotFromName<valueStr, MValueTypeI>::type;

using MMapSlot = typename ManagedMapSlotFromId<MMapIdBIF, MMapType>::type;
using MMapSlot2 = typename ManagedMapSlotFromName<mapStr, bool, MValueTypeI, MValueTypeF>::type;

using InvalidMSlotsSet = ManagedSlotsSet<MValueId>;
using MSlotsSet = ManagedSlotsSet<MValueSlot, MMapSlot>;

//ManagedValueSlot:
//nejde vytvorit z nekompatibilni dvojice [ManagedValueId, ManagedValueType]
//using InvalidMValueSlot = typename ManagedValueSlotFromId<MValueId, MValueTypeF>::type;//this doesn't compile
//
//jde vytvorit z kompatibilni dvojice [ManagedValueId, ManagedValueType]
//jde vytvorit z dvojice [jmeno, ManagedValueType]
//zna sve jmeno
static_assert(MValueSlot::name == valueStr);
//zna svuj typ hodnoty
static_assert(std::is_same_v<typename MValueSlot::valueType, int>);
//zna svuj typ managementu
static_assert(std::is_same_v<typename MValueSlot::managementType, DummyManagementType<int>>);
//zna svuj managedType
static_assert(std::is_same_v<typename MValueSlot::managedType, ManagedValueType<int, DummyManagementType<int>>>);
//vytvoren ruznymi zpusoby dava stejny typ
static_assert(std::is_same_v<MValueSlot, MValueSlot2>);

//ManagedMapSlot:
//nejde vytvorit z nekompatibilni dvojice [ManagedMapId, ManagedMapType]
//using InvalidMMapSlot = typename ManagedMapSlotFromId<MMapIdIDF, MMapType>::type;//this doesn't compile
//
//jde vytvorit z kompatibilni dvojice [ManagedMapId, ManagedMapType]
//jde vytvorit z trojice [jmeno, typKlice, ManagedValueType...]
//zna sve jmeno
static_assert(MMapSlot::name == mapStr);
//zna typ sveho klice
static_assert(std::is_same_v<typename MMapSlot::keyType, bool>);
//zna tuple svych ManagedValueTypes
static_assert(
    std::is_same_v<typename MMapSlot::valueTypesTuple, std::tuple<MValueTypeI, MValueTypeF>>
    || std::is_same_v<typename MMapSlot::valueTypesTuple, std::tuple<MValueTypeF, MValueTypeI>>);
//zna set svych ManagedValueTypes
static_assert(type::is_same_set<typename MMapSlot::valueTypesSet, type::makeSetT<MValueTypeI, MValueTypeF>>);
//zna svuj managedType
static_assert(std::is_same_v<typename MMapSlot::managedType, ManagedMapType<bool, MValueTypeI, MValueTypeF>>
    || std::is_same_v<typename MMapSlot::managedType, ManagedMapType<bool, MValueTypeF, MValueTypeI>>);
//vytvoren ryznymi zpusoby dava stejny typ
//TODO: toto neni spravny test ani spravna idea: dva MapSloty maji predstavovat stejny typ
//  bez ohledu na to, v jakem poradi byly v definici uvedeny typy jednotlivych ManagedValues
//  to pomoci std::is_same nelze implementovat, potrebuju vlastni grid::is_same trait
//  a jemu odpovidajici grid::is_same_v helper.
static_assert(std::is_same_v<MMapSlot, MMapSlot2>);

//ManagedSlotsSet:
//nejde vytvorit s typem, kterej neni ManagedSlot
//InvalidMSlotsSet invSlotSet{};//this shouldn't compile
//
//jde vytvorit s ManagedXXXSlots
//umi vratit sve Slots jako tuple
static_assert(
    std::is_same_v<typename MSlotsSet::tuple, std::tuple<MValueSlot, MMapSlot>>
    || std::is_same_v<typename MSlotsSet::tuple, std::tuple<MMapSlot, MValueSlot>>); 
//umi vratit sve Slots jako Set
static_assert(type::is_same_set<typename MSlotsSet::set, type::makeSetT<MValueSlot, MMapSlot>>);

}
