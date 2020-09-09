#include <cstddef>
#include <cassert>
#include <cstring>

#include "oneginC.h"

namespace oneginC {

    inline void swap(void *const a, void *const b, std::size_t size) {
        assert(a);
        assert(b);
        void *tmp = malloc(size);
        memcpy(tmp, a, size);
        memcpy(a, b, size);
        memcpy(b, tmp, size);
        free(tmp);
    }

    std::size_t partition(void *const data, std::size_t cnt, std::size_t size, const CompType cmp) {
        assert(data);
        assert(cmp);
        void *const val = malloc(size);
        memcpy(val, (char *) data + (cnt / 2) * size, size);
        std::size_t first = 0;
        std::size_t last = cnt - 1;
        while (first <= last) {
            while (cmp((char *) data + first * size, val) < 0) first++;
            while (cmp(val, (char *) data + last * size) < 0) last--;
            if (first >= last) break;
            oneginC::swap((char *) data + first++ * size, (char *) data + last-- * size, size);
        }
        free(val);
        return last;
    }

    void sort(void *const data, std::size_t cnt, std::size_t size, const CompType cmp) {
        assert(data);
        assert(cmp);
        std::size_t p = oneginC::partition(data, cnt, size, cmp);
        if (cnt > 2) {
            oneginC::sort(data, p + 1, size, cmp);
            oneginC::sort((char *) data + (p + 1) * size, cnt - (p + 1), size, cmp);
        }
    }

}