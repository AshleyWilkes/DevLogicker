#pragma once

namespace logicker::core {

//pointa teto tridy je v tom, mit futral nad types_util::SetT<MappingId>,
class GridType {
  private:
    template<typename G1, typename G2>
    friend struct is_sub_grid_type;

    using hana_set = void;
};

}
