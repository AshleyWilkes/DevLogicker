#pragma once

namespace logicker::core::operation::transform {

//na ten Transform jsem si vymyslel type trait:
//relation::determines<Relation, int>
//ten necht ma bool hodnotu true, pokud Relation ma tu vlastnost,
//ze jednoznacne urcuje hodnotu i-teho parametru, jsou-li zadany hodnoty
//vsech ostatnich parametru
//
//to be clear: hodnota true toho traitu neimplikuje, ze existuje implementace,
//ktera by umela hledanou hodnotu spocitat; pouze implikuje, ze by takova implementace
//mohla existovat
template<typename S, typename T>
class TransformSet {
  //assertnout, ze T je transform z typu parametr-oznacujici-prvek-z-S na nejaky typ O
  static_assert(true);
};

}
