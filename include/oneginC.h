#pragma once
#include <cstddef>



namespace oneginC {
    using CompType = int (*)(const void * a, const void * b);
    void sort(void* data, std::size_t cnt, std::size_t size, CompType cmp);
}