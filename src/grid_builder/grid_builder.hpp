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
template<auto Id, typename OutputsTo_, typename... Reads_>
class InitStep {
  public:
    using OutputsTo = OutputsTo_;
    using Reads = std::tuple<Reads_...>;
    using Requires = std::tuple<OutputsTo, Reads>;
};

using ExampleSizeInitStep = InitStep<catalogue::mapping_id::Size, InputField<sizeStr>>;

template<typename ValueT, char* inputFieldId = givensStr>
using ExampleCentersInitStep = InitStep<catalogue::mapping_id::Centers<ValueT>, InputField<inputFieldId>, catalogue::mapping_id::Topology>;

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
