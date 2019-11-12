#include "grid_builder/grid_builder.hpp"

namespace {
}

//Potrebuju sadu testu na grid_builder, ze mi buildi podle receptu, jak ma.
//
//Na to jednak potrebuju unit testy; kazdy initstep jedna test classa, klasika.
//
//Dvak na to chci sadu integracnich testu postavenych nad inputy,
//vydestilovanymi z bookletu MS2018. Takovyhle test vyzaduje 3 vstupy:
//- GridRecipe, umoznujici Grid vybuildit
//- nazev souboru v adresari ???, obsahujici input, umoznujici Grid spravne naplnit
//- natvrdo rucne zadany obsah Gridu, na nejz predchozi dva inputy vedou
//    a jenz ma byt v tele testu porovnan na rovnost (nebo inkluzi?) s nactenym
//    Gridem. Zde je drobounky problemek v tom, ze na toto natvrde rucni zadavani
//    obsahu Gridu nejsem zarizen.
//
//Nicmene zaridim-li se, pak toto je (po doplneni dostatecneho mnozstvi vstupnich
//trojic muze byt) vcelku presvedcivy test toho, ze GridBuilder a InitStepy
//jsou spravne implementovane!
//
//Inu tedy rucni zadavani obsahu Gridu:
//obsah sestava z dvojic
//Mapping (MappingId + MappingType)
//obsah tohoto mappingu: konstanta (value nebo domena) nebo mapa (Key => konstanta), dle vlastnosti MappingId
//
//Takze treba takto
//ReadOnlyMapping<catalogue::mapping::Size> mp{ 5 };
