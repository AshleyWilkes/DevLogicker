#pragma once

#include "core/puzzle.hpp"
#include "core/item/topology/square.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

char minesCountStr[] = "minesCount";

class MinesImpl {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Size,
      init::Square,
      init::Scalar<int, minesCountStr>,
      init::Centers<int>
    >;
};

using Mines = core::Puzzle<MinesImpl>;

}
