#include "grid_builder/grid_builder.hpp"

//znacna cast GridBuildera obnasi compile time typove vypocty
//proto nektere testy zde obshauji pouze staticke checky a uspechem
//je u nich uz to, kdyz se je podari uspesne zkompilovat

namespace {

namespace hana = boost::hana;

//trivialni zamerne nedefinovana templata, kterou lze jednoduse
//pouzit pro zkoumani toho, jake typy zpusobily compile failure
//u BOOST_HANA_CONSTANT_CHECK; poor man's solution toho, ze
//BOOST_HANA_CONSTANT_CHECK nic nerekne
template<typename T>
class TD;

using Recipe = logicker::grid_builder::GridRecipe<
  logicker::grid_builder::ExampleSizeInitStep, logicker::grid_builder::ExampleCentersInitStep<int>>;
using MinimalGridType = typename Recipe::MinimalGridType;
//BOOST_HANA_CONSTANT_CHECK( hana::to<hana::set_tag>( MinimalGridType{} )
    //== hana::make_set());
//TD<MinimalGridType> disp;


}
