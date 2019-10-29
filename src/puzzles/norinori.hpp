#pragma once

#include "core/puzzle.hpp"
#include "core/item/topology/rectangle.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

class NorinoriImpl {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Width,
      init::Height,
      init::Rectangle,
      init::Regions<>,
      init::Centers<bool>
    >;
};

using Norinori = core::Puzzle<NorinoriImpl>;

}
