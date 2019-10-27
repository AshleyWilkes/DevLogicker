#pragma once
#include "catalogue/mapping_id/mapping_id.hpp"
#include "core/mapping_id.hpp"
#include "input/input.hpp"
#include "types_util/types_util.hpp"

namespace logicker::grid_builder {

char sizeStr[] = "size";
char givensStr[] = "givens";

template<char* id>
struct InputField {};

//jednoduche InitStepy maji 3 kroky:
//1) precti vstupy (z inputu nebo gridu, vzdy konstanty)
//2) zavolej nejakou metody s nactenymi vstupy
//3) vysledek metody zapis do dane konstanty v gridu
//
//ten slozitej InitStep ma taky 3 kroky:
//1) nacti (cte input i grid)
//2) zavolej metodu (ta metoda ale zalezi na formatu vstupu z input)
//3) zapis (to je stejne jako u jednoducheho stepu, akorat mam mapujici nikoli konstantni mapping,
//    coz by ale nemel byt vubec zadny rozdil
template<typename OutputsTo_, typename OperationId_, typename... Reads_>
class InitStep {
  public:
    using OutputsTo = OutputsTo_;
    using Reads = std::tuple<Reads_...>;
    using Requires = std::tuple<OutputsTo, Reads>;

    using OperationId = OperationId_;
};

//using SizeInitStep = InitStep<catalogue::mapping_id::Size, catalogue::transform::Identity<int>, InputField<sizeStr>>;

//v nasledujicim ???XXX??? ma popsat, ze prirazuju hodnotu kazdemu Centers fieldu, a to bud nactenou, ktera musi byt z jedne domainy, anebo kdyz neni co nacist, tak druhou domainu. Toto se zda ponekud complicated na to, aby si to nezaslouzilo rozbit na kousky!
//v jadru je Operation, ktera cte hodnoty ze vstupu. Ta ma jako vystup mapu <Coords, Value>, muze se nejak jmenovat a muze mit nekolik implementaci ocekavajicich ruzny format vstupu (stream of Values vs stream of Coords, Value objects. (Soucasti vstupu by mohla byt i domaina pripustnych hodnot za ucelem odhaleni erroru pri parsovani!)
//nad cteci operaci je futral, kterej ocheckuje, jestli nactena mapa obsahuje hodnoty pro vsechny coordsy,ktere existuji; pokud ne, pokusi se doplnit chybejici coordsy vhodnou domenou.
//Cteci operace pracujici se streamem of Values muze byt udelana tak, ze prirazuje coorsum pouze hodnoty, ktere se ji povedlo rozparsovat, kdyz se parse nepovede, prislusna Coordsa zustane prazdna
//template<typename ValueT, char* inputFieldId = givensStr>
//using CentersInitStep = InitStep<catalogue::mapping_id::Centers<ValueT>, ???XXX???, InputField<inputFieldId>, catalogue::mapping_id::Topology>;

template<typename... InitSteps>
class GridRecipe {
  public:
    using MinimalGridType = type::set_union_t<type::SetT<typename InitSteps::Requires>...>;
};

template<typename GridRecipe, typename BuiltType = typename GridRecipe::MinimalGridType>
class GridBuilder {
  public:
    BuiltType build( const core::input::input& );
};

}
