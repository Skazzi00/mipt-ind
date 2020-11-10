#pragma once
#include <cstddef>

namespace mipt {
    template<std::size_t... I>
    class index_sequence {};

    template<std::size_t N, std::size_t ...I>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, I...> {};

    template<std::size_t ...I>
    struct make_index_sequence<0, I...> : index_sequence<I...> {};


    template<class Iter, class T>
    constexpr Iter find(Iter first, Iter last, const T& value)
    {
        for (; first != last; ++first) {
            if (*first == value) {
                return first;
            }
        }
        return last;
    }
}