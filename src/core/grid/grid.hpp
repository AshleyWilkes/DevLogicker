#pragma once
#include "core/grid/managed_id.hpp"
#include "core/grid/managed_type.hpp"
#include "core/grid/managed_slot.hpp"
#include "core/grid/managed_value.hpp"
#include "core/grid/managed_map.hpp"

#include <boost/hana.hpp>

//chtelo by to mit jmena pro nekolik stupnu na ceste od ManagedValueId
//k full instantiated Gridu:
//
//ManagedValueId -- id, oznacujici 1 managed hodnotu: [jmeno, typHodnoty]
//ManagedMapId -- id, oznacujici mapu managed hodnot: [jmeno, typKlice, typHodnoty...]
//ManagedIdsSet -- mnozina, jejimiz prvky jsou ManagedValueIds a ManagedMapIds
//
//ManagementType -- zpusob, jak 1 hodnotu managuju; zda se, ze (na sebe kolme) moznosti jsou 
//    jednoduse store hodnoty VS domaina, z niz lze vyskrtavat
//    optional (muze hodnotu nemit) VS obligatory (musi mit hodnotu, jinak error)
//
//ManagedValueType -- dvojice [typHodnoty, ManagementType]
//ManagedMapType -- dvojice [typKlice, ManagedValueType...]
//
//ManagedValueSlot -- kompatibilni dvojice [ManagedValueId, ManagedValueType], 
//      jinak tez dvojice [jmeno, ManagedValueType],
//      jinak tez trojice [jmeno, typHodnoty, ManagementType]
//ManagedMapSlot -- kompatibilni dvojice [ManagedMapId, ManagedMapType],
//      jinak tez trojice [jmeno, typKlice, ManagedValueType...]
//
//ManagedSlotsSet -- mnozina, jejimiz prvky jsou ManagedValueSlots a ManagedMapSlots
//
//ManagedXXXType je instancializovatelny na ManagedXXX
//
//Grid je instancializovany ManagedSlotsSet v tom smyslu, ze kazdemu ManagedXXXSlotu je prirazena jedna instance ManagedXXX

namespace logicker::core::grid {

//make_slot_instance ma vzit Slot a vratit instanci ManagedXXX
template<typename Slot>
struct SlotInstance;

template<auto name_, typename valueType_, typename managementType_>
struct SlotInstance<ManagedValueSlotImpl<name_, valueType_, managementType_>> {
  public:
    using type = ManagedValue<valueType_, managementType_>;
    using Id = ManagedValueId<name_, valueType_>;
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct SlotInstance<ManagedMapSlotImpl<name_, keyType_, std::tuple<valueTypes_...>>> {
  public:
    using type = ManagedMapImpl<keyType_, std::tuple<valueTypes_...>>;
    using Id = ManagedMapId<name_, keyType_, valueTypes_...>;
};

template<typename MSlot>
struct MSlotPairBuilder;

template<auto name_, typename valueType_, typename managementType_>
struct MSlotPairBuilder<ManagedValueSlotImpl<name_, valueType_, managementType_>> {
  public:
    static inline auto value = boost::hana::make_pair(boost::hana::type_c<ManagedValueId<name_, valueType_>>, typename SlotInstance<ManagedValueSlotImpl<name_, valueType_, managementType_>>::type{});
};

template<auto name_, typename keyType_, typename... valueTypes_>
struct MSlotPairBuilder<ManagedMapSlotImpl<name_, keyType_, std::tuple<valueTypes_...>>> {
  public:
    static inline auto value = boost::hana::make_pair(boost::hana::type_c<ManagedMapId<name_, keyType_, typename valueTypes_::ValueType...>>, typename SlotInstance<ManagedMapSlotImpl<name_, keyType_, std::tuple<valueTypes_...>>>::type{});
};

template<typename MSlot>
auto MSlotPair = MSlotPairBuilder<MSlot>::value;

template<typename... MSlot>
class GridImpl;

//Toto ma reprezentovat tuple instancializovanych ManagedXXXu, kazdy oindexovany svym ManagedId. Right?
//takze to asi musim vyst pres run-time hana::mapu, ktera ma jako klice hana::type_c<>
//  prislusneho ManagedId. Ke kazdemu klici nalezi hodnota typu ManagedXXX, vytvorena pomoci
//  SlotInstance, coz patri sem do tohoto souboru, protoze je prvni, ktery dava dohromady
//  sloty a instance
//
template<typename... MSlot>
class GridImpl<std::tuple<MSlot...>> {
  private:
    //using InnerTuple = std::tuple<typename SlotInstance<MSlot>::type...>;
    //InnerTuple instances_;
    using Instances = decltype(boost::hana::make_map(MSlotPair<MSlot>...));
    Instances instances_ = boost::hana::make_map(MSlotPair<MSlot>...);
  public:
    using MSlotsSet = type::SetT<std::tuple<MSlot...>>;
    using MIdsTuple = std::tuple<typename SlotInstance<MSlot>::Id...>;

    template<typename MId>
    auto& get() {
      return instances_[boost::hana::type_c<MId>].getInstance();
    }

    const auto& getInstances() const { return instances_; }
};

template<typename ArgsTuple, template<typename> typename... ManagementType>
struct Grid;

template<typename... MIds, template<typename> typename ManagementType>
struct Grid<std::tuple<MIds...>, ManagementType> {
  public:
    using type = GridImpl<typename ManagedSlotsSetFromIds<ManagedIdsSet<MIds...>, ManagementType>::tuple>;
};

template<typename... MIds, template<typename> typename ManagementType>
struct Grid<ManagedIdsSet<MIds...>, ManagementType> {
  public:
    using type = GridImpl<typename ManagedSlotsSetFromIds<ManagedIdsSet<MIds...>, ManagementType>::tuple>;
};

template<typename... MSlots>
struct Grid<ManagedSlotsSet<MSlots...>> {
  public:
    using type = GridImpl<typename ManagedSlotsSet<MSlots...>::tuple>;
};

//gridy jsou povazovany za stejny, kdyz maji stejnej set MSlotu. Tj. stejny id, type *i* management!
//jeden grid je subgridem druhyho, kdyz kazdej jeho MSlot je subslotem odpovidajiciho slotu v druhym Gridu
//  Subslot S1 slotu S2 ma stejne id a typ jako S2 a dale "kazda hodnota, ktera se da napsat do S1, se da
//      napsat i do S2". Toto subslotovani vyzaduje specializovat na konkretni typy ManagementTypu,
//      takze vlastne neni jasne, kam patri jeho implementace...
template<typename G1, typename G2>
inline constexpr bool is_same_grid_v = type::is_same_set<typename G1::MSlotsSet, typename G2::MSlotsSet>;

template<typename MId, typename MIdsTuple>
class HasSuperMIdIn;

//  true, kdyz MId je subMId nejakeho MId z G2
//  false jinak
//tj. prochazet MIds2 a zorovat vysledky operace, ktera pro MId a MId2 vrati
//  true, kdyz MId je subMId MId2
//  false jinak
template<typename MId, typename... MIds2>
class HasSuperMIdIn<MId, std::tuple<MIds2...>> {
  public:
    static inline constexpr bool value = ( ... || is_sub_MId_v<MId, MIds2> );
};

template<typename G1, typename G2>
class IsSubgrid;

//grid G1 je subgridem G2, kdyz kazdy jeho MId ma v G2 supergrid:
//MValueId musi byt pritomno v G2 ve stejne forme vcetne management typu
//MMapId musi byt v G2 ve forme, ktera pripousti stejne veci (muze jich pripoustet vic); toto nabizi
//  ruzne moznosti s ohledem na domeny, volim asi moznost domeny ignorovat a pozadovat v G2 stejne
//  datove typy v G2
//bylo by asi vhodne delegovat rozhodovani o jednotlivem MId na prislusne implementace teto
//  promenne pro MValueId a MMapId v managed_id.hpp
//cili zde chci prochazet MIds z G1 a zandovat vysledky operace, ktera pro dane MId z G1 vrati
//  true, kdyz MId je subMId nejakeho MId z G2
//  false jinak
template<typename... MIds1, typename... MIds2>
class IsSubgrid<std::tuple<MIds1...>, std::tuple<MIds2...>> {
  public:
    static inline constexpr bool value = ( ... && HasSuperMIdIn<MIds1, std::tuple<MIds2...>>::value );
};

template<typename G1, typename G2>
inline constexpr bool is_subgrid_v = IsSubgrid<typename G1::MIdsTuple, typename G2::MIdsTuple>::value;

template<typename Keys, typename Lhs, typename Rhs>
bool compareInstances( const Keys& keys, const Lhs& lhs, const Rhs& rhs ) {
  if constexpr ( boost::hana::is_empty( keys ) ) {
    return true;
  } else {
    auto key = boost::hana::front( keys );
    if ( lhs[ key ] <= rhs[ key ] ) {
      return compareInstances( boost::hana::drop_front( keys ), lhs, rhs );
    } else {
      return false;
    }
  }
}

//pozadavky:
//rhs musi obsahovat vsechny MappingIds, ktere obsahuje lhs -- static_assert nad ManagedIdsSets
//kazdy Mapping v lhs musi byt podmnozinou Mappingu v rhs -- volani porovnani na mappingach
template<typename... MSlots1, typename... MSlots2>
bool operator<=(
    const GridImpl<std::tuple<MSlots1...>>& lhs,
    const GridImpl<std::tuple<MSlots2...>>& rhs ) {
  using MIdsSet1 = typename ManagedSlotsSet<MSlots1...>::managedIdsSet::set;
  using MIdsSet2 = typename ManagedSlotsSet<MSlots2...>::managedIdsSet::set;
  static_assert( type::is_subset<MIdsSet1, MIdsSet2> );
  
  auto keys = boost::hana::keys( lhs.getInstances() );
  return compareInstances( keys, lhs.getInstances(), rhs.getInstances() );
}

}
