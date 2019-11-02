#pragma once

#include "core/puzzle.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

class SudokuClassicImpl {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Size,//nene, tohle ma byt proste 9
      init::Square,
      init::SudokuSmallSquares,
      init::Centers<core::item::Alphanumeric>
    >;
};

using SudokuClassic = core::Puzzle<SudokuClassicImpl>;

}
