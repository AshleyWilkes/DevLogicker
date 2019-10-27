#include "grid_builder/grid_builder.hpp"

//znacna cast GridBuildera obnasi compile time typove vypocty
//proto nektere testy zde obshauji pouze staticke checky a uspechem
//je u nich uz to, kdyz se je podari uspesne zkompilovat

//trivialni zamerne nedefinovana templata, kterou lze jednoduse
//pouzit pro zkoumani toho, jake typy zpusobily compile failure
//u BOOST_HANA_CONSTANT_CHECK; poor man's solution toho, ze
//BOOST_HANA_CONSTANT_CHECK nic nerekne
template<typename T>
class TD;

namespace {

namespace hana = boost::hana;

//using Recipe = logicker::grid_builder::GridRecipe<
  //logicker::grid_builder::ExampleSizeInitStep, logicker::grid_builder::ExampleCentersInitStep<int>>;
//using MinimalGridType = typename Recipe::MinimalGridType;
//BOOST_HANA_CONSTANT_CHECK( hana::to<hana::set_tag>( MinimalGridType{} )
    //== hana::make_set());
//TD<MinimalGridType> disp;


}


//NEWER
//Potrebuju sadu testu na grid_builder, ze mi buildi podle receptu, jak ma.
//
//Na to tedy jednak chci testy jednotlivych InitStepu; techto InitStepu
//je (bude) ale vcelku znacne omezene mnozstvi, vetsina realne pouzivanych
//InitStepu jsou aliasy, obsahujici vhodne nastavene parametry.
//
//Dvak na to tedy chci normalne natvrdo sadu testu postavenych nad inputy,
//vydestilovanymi z bookletu MS2018. Takovyhle test vyzaduje 3 vstupy:
//- GridRecipe, umoznujici Grid vybuildit
//- nazev souboru v adresari ???, obsahujici input, umoznujici Grid spravne naplnit
//- natvrdo rucne zadanu obsah Gridu, na nejz predchozi dva inputy vedou
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
