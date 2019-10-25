#pragma once

namespace logicker::core {

template<typename S1, typename S2>
auto isSubset = hana::difference( S1{}, S2{} ) == hana::make_set();

template<typename G1, typename G2>
struct is_sub_grid_type {
  public:
    bool value = isSubset<G1, G2>();
};

template<typename G1, typename G2>
inline constexpr bool is_sub_grid_type_v = is_sub_grid_type<G1, G2>::value;

template<typename G1, typename G2>
struct is_same_grid_type {
  public:
    bool value = is_sub_grid_type_v<G1, G2> && is_sub_grid_type_v<G2, G1>;
};

template<typename G1, typename G2>
inline constexpr bool is_same_grid_type_v = is_same_grid_type<G1, G2>::value;

//pointa teto tridy je v tom, mit futral nad hana::set<MappingId>,
//ktery umozni vytvaret tyto typy ruzne, napr. pomoci std::tuple nebo primo
//tim hana::setem, pokud je to pohodlnejsi
//
//taky je pravda, ze hana::set je implementacni detail, takze mit interface je dobre
class GridType {
  private:
    template<typename G1, typename G2>
    friend struct is_sub_grid_type;

    using hana_set = void;
};

}
