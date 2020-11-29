#pragma once
#include <cstddef>
#include <utility>

namespace mipt {
template<std::size_t... I>
class index_sequence {};

template<std::size_t N, std::size_t ...I>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, I...> {};

template<std::size_t ...I>
struct make_index_sequence<0, I...> : index_sequence<I...> {};

template<class It1, class It2>
void iswap(It1 a, It2 b) {
  using std::swap;
  swap(*a, *b);
}

}