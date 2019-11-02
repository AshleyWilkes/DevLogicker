#include "types_util/types_util.hpp"
//Typy a compile time prace s nimi by si zaslouzily vlastni knihovnu s promyslenym
//designem. Ten v tuto chvili z casovych duvodu nepripada v uvahu, cili volim pragmaticky
//pristup naprasit jakkoli utility, ktere potrebuju, a zaradit do TODO. Protoze tech utilit,
//ktere potrebuju, neni mnoho, budou vsechny testy tady v jednom souboru.

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

//test: lze vytvorit SetT z prazdnyho std::tuple
//test: lze vytvorit SetT z neprazdnyho std::tuple, v nemz se neopakuji prvky
//test: nelze vytvorit SetT z neprazdnyho std::tuple, v nemz se opakuji prvky
//test: lze vytvorit SetT z SetT
//
//test: SetT vytvoreny ze SetT is_same_set jako ten puvodni
//test: dva SetT vytvorene z tuplu s prohozenyma prvkama jsou same_set
//
//test: SetT z prazdnyho std::tuple je subset SetTu z neprazdnyho std::tuple
//test: SetT z tuplu s jednim prvkem je subset SetTu z tuplu s dvema prvky, kdyz je ten jeden prvek v obout tuplech
//test: SetT z tuplu s jednim prvkem neni subset SetTu z tuplu s dvema prvky, kdyz ten jeden prvek v druhym tuplu neni
//
//test: set_union z 2 SetTu bez opakovani
//test: set_union z 2 SetTu s opakovanim
//test: set_union z 4 SetTu

namespace {

namespace type = logicker::type;

using TupleEmpty = std::tuple<>;
using Tuple_C = std::tuple<char>;
using Tuple_D = std::tuple<double>;
using Tuple_I = std::tuple<int>;
using Tuple_II = std::tuple<int, int>;
using Tuple_ID = std::tuple<int, double>;
using Tuple_DI = std::tuple<double, int>;

//test: lze vytvorit SetT z prazdnyho std::tuple
using EmptySet = type::SetT<TupleEmpty>;

//test: lze vytvorit SetT z neprazdnyho std::tuple, v nemz se neopakuji prvky
using NonEmptySet = type::SetT<Tuple_I>;

//TODO:test: nelze vytvorit SetT z neprazdnyho std::tuple, v nemz se opakuji prvky

//test: lze vytvorit SetT z SetT
using CopiedSet = type::SetT<NonEmptySet>;

//test: SetT umi vratit svuj obsah jako std::tuple
static_assert(std::is_same_v<typename NonEmptySet::tuple, Tuple_I>);

//test: SetT vytvoreny ze SetT is_same_set jako ten puvodni
static_assert(type::is_same_set<NonEmptySet, CopiedSet>);

//test: dva SetT vytvorene z tuplu s prohozenyma prvkama jsou same_set
static_assert(type::is_same_set<type::SetT<Tuple_ID>, type::SetT<Tuple_DI>>);

//test: SetT z prazdnyho std::tuple je subset SetTu z neprazdnyho std::tuple
static_assert(type::is_subset<EmptySet, NonEmptySet>);

//test: SetT z tuplu s jednim prvkem je subset SetTu z tuplu s dvema prvky, kdyz je ten jeden prvek v obout tuplech
static_assert(type::is_subset<type::SetT<Tuple_I>, type::SetT<Tuple_ID>>);

//test: SetT z tuplu s jednim prvkem neni subset SetTu z tuplu s dvema prvky, kdyz ten jeden prvek v druhym tuplu neni
static_assert(! type::is_subset<type::SetT<Tuple_C>, type::SetT<Tuple_ID>>);

//test: set_union z 2 SetTu bez opakovani
static_assert(type::is_same_set<type::set_union_t<type::SetT<Tuple_D>, type::SetT<Tuple_I>>, type::SetT<Tuple_ID>>);

//test: set_union z 2 SetTu s opakovanim
static_assert(type::is_same_set<type::set_union_t<type::SetT<Tuple_D>, type::SetT<Tuple_ID>>, type::SetT<Tuple_ID>>);

//test: set_union z 4 SetTu
static_assert(type::is_same_set<type::set_union_t<type::SetT<Tuple_D>, type::SetT<Tuple_I>, type::SetT<Tuple_ID>, type::SetT<Tuple_DI>>, type::SetT<Tuple_ID>>);
}
