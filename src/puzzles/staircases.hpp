#pragma once

#include "catalogue/init_step/init_step.hpp"
#include "core/item/alphanum.hpp"
#include "core/puzzle.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

class StaircasesImpl {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Size,
      init::Square,
      init::Regions<>,
      init::Centers<core::item::Alphanumeric>
    >;
};

using Staircases = core::Puzzle<StaircasesImpl>;

}
