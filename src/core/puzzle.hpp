#pragma once

#include "catalogue/init_step/init_step.hpp"
#include "grid_builder/grid_recipe.hpp"

namespace logicker::core {

template<typename PuzzleImpl>
class Puzzle {
  public:
    using GridRecipe = typename PuzzleImpl::GridRecipe;
    using MappingIdSet = typename GridRecipe::MinimalGridType;
};

}
