#pragma once
/*#include<concepts>

namespace logicker::core {

template<typename T>
concept Item = std::equality_comparable<T>;

}*/

#include <type_traits>
namespace logicker::core {

template<typename T>
concept bool Item = requires(
    const std::remove_reference_t<T>& t1,
    const std::remove_reference_t<T>& t2) {
  { t1 == t2 } -> bool;
  { t1 != t2 } -> bool;
};

}
