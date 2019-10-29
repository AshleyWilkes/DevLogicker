#pragma once

#include "catalogue/item/magnet_field.hpp"
#include "core/puzzle.hpp"
#include "core/item/topology/rectangle.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

class MagnetsImpl {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Width,
      init::Height,
      init::Rectangle,
      init::Dominoes<>,
      init::UDirectionClues<std::pair<int,int>>,
      init::Centers<item::MagnetField>
    >;
};

using Magnets = core::Puzzle<MagnetsImpl>;

}
