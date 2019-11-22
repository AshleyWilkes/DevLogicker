#include "grid_builder/grid_builder.hpp"

namespace {
}

//Potrebuju sadu testu na grid_builder, ze mi buildi podle receptu, jak ma.
//
//Na to jednak potrebuju unit testy; kazdy initstep jedna test classa, klasika.
//
//Dvak na to chci sadu integracnich testu postavenych nad inputy,
//napr. vydestilovanymi z bookletu MS2018. Presvedciva sada testu musi
//jit postavit nad dvojicemi volani build_assignment + build_solution.
//Takovy test vyzaduje 3 vstupy:
//- GridRecipe, umoznujici Grid vybuildit
//- nazev souboru v adresari ???, obsahujici input, umoznujici oba Gridy spravne naplnit
//- informaci o tom, zda solution odpovida zadani; tuto poskytuju "meta" zpusobem:
//    budou dva adresare, jeden s validnimi zadanimi, jeden s nevalidnimi
//    a budou dva testy nebo dve skupiny testu, jeden pro validni, jeden pro nevalidni
//

