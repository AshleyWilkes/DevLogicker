#pragma once

#include "core/puzzle.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

char starsCountStr[] = "starsCount";

class StarbattleImple {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Size,
      init::Square,
      init::Scalar<int, starsCountStr>,
      init::Regions<>,
      init::Centers<int>
    >;
};

using Starbattle = core::Puzzle<StarbattleImple>;

}
