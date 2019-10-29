#pragma once

#include "catalogue/item/elements.hpp"
#include "core/puzzle.hpp"
#include "core/item/topology/rectangle.hpp"

namespace logicker::puzzle {

using namespace logicker::catalogue;

template<typename Topology>
class ElementsImpl;

template<>
class ElementsImpl<core::item::topology::Rectangle> {
  public:
    using GridRecipe = grid_builder::GridRecipe<
      init::Width,
      init::Height,
      init::Rectangle,
      init::Regions<>,//mapping_id::Regions <= input.get("regions");
      init::RegionValues<item::Elements>
    >;
};

template<typename Topology = core::item::topology::Rectangle>
using Elements = core::Puzzle<ElementsImpl<Topology>>;

}
