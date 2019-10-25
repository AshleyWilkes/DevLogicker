#pragma once
#include <boost/hana.hpp>
#include <tuple>

//Typy a compile time prace s nimi by si zaslouzily vlastni knihovnu s promyslenym
//designem. Ten v tuto chvili z casovych duvodu nepripada v uvahu, cili volim pragmaticky
//pristup naprasit jakkoli utility, ktere potrebuju, a zaradit do TODO. Protoze tech utilit,
//ktere potrebuju, neni mnoho, budou vsechny tady v jednom souboru.

//utility ocekavaji jako template argumenty instance templaty SetT, ktera je inicializovatelna
//template argumentem std::tuple anebo SetT; SetT ma membera value, ktery obsahuje samotnou mnozinu,
//typ tohoto membera je implementacni detail

//tvorba:
//template<typename... Ts>
//SetT<std::tuple<Ts...>>
//
//template<typename... Ts>
//SetT<SetT<Ts...>>
//
//utility:
//template<typename SetT1, typename SetT2>
//inline constexpr bool is_same_set
//
//template<typename SubSetT, typename SuperSetT>
//inline constexpr bool is_subset
//
//template<typename... SetTs>
//using set_union_t

namespace logicker::type {

namespace hana = boost::hana;

template<typename T>
class SetT;

template<typename... Ts>
class SetT<std::tuple<Ts...>> {
  public:
    using type = decltype( hana::make_set( hana::type_c<Ts>... ) );
    static constexpr type value = hana::make_set( hana::type_c<Ts>... );
};

template<typename... Ts>
class SetT<SetT<Ts...>> {
  public:
    using type = typename SetT<Ts...>::type;
    static constexpr type value = SetT<Ts...>::value;
};

template<typename... Ts>
class SetT<hana::set<Ts...>> {
  public:
    using type = hana::set<Ts...>;
    static constexpr type value = hana::make_set( Ts{}... );
};

template<typename SubSetT, typename SuperSetT>
inline constexpr bool is_subset = hana::difference( SubSetT::value, SuperSetT::value ) == hana::make_set();

template<typename SetT1, typename SetT2>
inline constexpr bool is_same_set = is_subset<SetT1, SetT2> && is_subset<SetT2, SetT1>;

template<typename... SetTs>
class SetUnion;

template<typename SetT1, typename SetT2>
class SetUnion<SetT1, SetT2> {
  public:
    using result = SetT<decltype( hana::union_( SetT1::value, SetT2::value ) )>;
};

template<typename SetT1, typename... SetTs>
class SetUnion<SetT1, SetTs...> {
  public:
    using result = SetT<decltype( hana::union_( SetT1::value, SetUnion<SetTs...>::result::value ) )>;
};

template<typename... SetTs>
using set_union_t = typename SetUnion<SetTs...>::result;

}
