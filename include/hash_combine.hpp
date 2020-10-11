#pragma once

#include <cstddef>

namespace mipt {
    template<class T>
    inline void hash_combine(std::size_t &seed, const T &v) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6u) + (seed >> 2u);
    }
}