#include <fstream>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "input/json_input.hpp"
#include "grid_builder/grid_builder.hpp"

#include "puzzles/elements.hpp"
#include "puzzles/latin_square.hpp"
#include "puzzles/magnets.hpp"
#include "puzzles/mines.hpp"
#include "puzzles/norinori.hpp"
#include "puzzles/staircases.hpp"
#include "puzzles/starbattle.hpp"
#include "puzzles/sudoku_classic.hpp"

//takze tady je misto na integracni test vsech znamych puzzlu
//ve finalni verzi ma tady byt neco jako 1 parametrizovanej test case,
//kterej udela kompletni check vsech featur na jednom vstupnim Puzzlu,
//a tento test case se ma volat pro vsechny zname Puzzly.
//
//Pod pojmem kompletni check vsech featur si prozatim predstavuju to,
//ze podle instrukci v GridRecipe a inputu v zadanem souboru nacte Grid
//se zadanim, nacte Grid s resenim, overi, ze reseni je podmnozina zadani
//a ze reseni splnuje Constraints.
//
//A pod pojmem volat pro vsechny zname Puzzly se mysli, ze zde budou rucne
//vsechny ty Puzzly vyjmenovane, ke kazdemu bude rucne zadana cesta k testovacimu
//souboru.
//
//protoze k finalni verzi je daleko, budujeme tento test postupne spolu
//s featurama samotnejma, takze pro zacatek bude test case pouze buildit
//Grid a overovat, ze vysledek je kompatibilni s MappingIdSetem, uvedenym v Puzzlu
//jako standard.
//
//V teto fazi budu budovat zdrojaky (Puzzle classy a .lgc soubory) za ucelem
//prozkoumani zasadniho predpokladu, ktery zni:
//Vsechny initstepy maji trikrokovou strukturu
//Vetsina jich ma jako druhy krok jednoduchou operaci, kterou lze prirozene popsat
//  prostredky k popisovani Constraintu.
//Existuje velmi malo slozitych initstep operaci, ktere si zaslouzi byt soucasti
//  grid_buildera, a ne catalogue, a jsou to prave ty operace, kterymi se lisi
//  buildeni assignmentu od buildeni solutionu. A mozna, ze jsou to zaroven prave
//  ty operace, ktere inicializuji non-const Mappingy.
//
//Tedy jak receno, predpokladam, ze existuji konkretni initstep operace, ktere
//  prirozene vedou na rozdilne chovani pri buildeni ass a sol, coz pujde snadno
//  zohlednit tim, ze GridBuilder bude mit jednu build metodu pro ass a jednu pro sol,
//  v dusledku cehoz bude mozne mit pouze jeden recept, podle ktereho bude mozne
//  vybuildit obe moznosti (ass cte hodnoty "value", sol cte hodnoty "solution").
namespace {

using namespace logicker;

std::string dirname = "../test_puzzle_inputs/";

core::input::json_input load_input( const std::string& path ) {
  std::ifstream is{ path };
  std::string input_str{ std::istreambuf_iterator<char>( is ),
    std::istreambuf_iterator<char>() };
  return logicker::core::input::build_input( input_str );
}

template<typename Puzzle>
void run_puzzle_test( std::string filename ) {
  using GridBuilder = typename grid_builder::GridBuilder<typename Puzzle::GridRecipe>;
  //precti input file
  auto input = load_input( dirname + filename );
  //postav assgnment grid
  auto assignment_grid = GridBuilder::build_assignment( input );
  //over kompatibilitu typu assignment gridu
  static_assert( type::is_subset<typename Puzzle::MappingIdSet, decltype( assignment_grid )> );
  //postav solution grid
  auto solution_grid = GridBuilder::build_solution( input );
  //over kompatibilitu typu solution gridu
  static_assert( type::is_subset<typename Puzzle::MappingIdSet, decltype( solution_grid )> );
  //over, ze solutiona je podmnozina assignmentu
  //EXPECT_LE( solution_grid, assignment_grid );
  //ma nasledovat jeste assert, ze solutiona splnuje Constraint
  //
  //v dalsi fazi pripadaji v uvahu spatne solutiony, ktere nesplnuji Constraint
  //a moznost Solvera vyresit Puzzle z assignmentu
}

TEST(IntegrationTest, Puzzles) {
  run_puzzle_test<puzzle::LatinSquare<>>( "latin_square3x3.lgc" );
  run_puzzle_test<puzzle::Elements<>>( "elements.lgc" );
  run_puzzle_test<puzzle::Magnets>( "magnets.lgc" );
  run_puzzle_test<puzzle::Mines>( "mines.lgc" );
  run_puzzle_test<puzzle::Norinori>( "norinori.lgc" );
  run_puzzle_test<puzzle::Staircases>( "staircases.lgc" );
  run_puzzle_test<puzzle::Starbattle>( "starbattle.lgc" );
  run_puzzle_test<puzzle::SudokuClassic>( "sudoku_classic.lgc" );
}

}
