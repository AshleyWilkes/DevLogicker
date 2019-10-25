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

template<typename OutputsTo_, typename... Reads_>
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
