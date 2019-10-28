#pragma once

namespace logicker::grid_builder {

template<typename... InitSteps>
class GridRecipe {
  public:
    using MinimalGridType = type::set_union_t<type::SetT<typename InitSteps::Requires>...>;
};

}
