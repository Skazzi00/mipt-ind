#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "C/onegin.h"

#define SWAP(a, b, size)              \
  do {                                \
      size_t __size = (size);         \
      char *__a = (a), *__b = (b);    \
      do {                            \
          char __tmp = *__a;          \
          *__a++ = *__b;              \
          *__b++ = __tmp;             \
        } while (--__size > 0);       \
    } while (0)


static size_t partition(void *const data, size_t cnt, size_t size, const Comp_t cmp) {
    assert(data);
    assert(cmp);

    void *const val = malloc(size);
    memcpy(val, (char *) data + (cnt / 2) * size, size);

    size_t first = 0;
    size_t last = cnt - 1;
    while (first <= last) {
        while (cmp((char *) data + first * size, val) < 0) first++;
        while (cmp(val, (char *) data + last * size) < 0) last--;
        if (first >= last) break;
        SWAP((char *) data + first++ * size, (char *) data + last-- * size, size);
    }

    free(val);
    return last;
}

void ON_sort(void *const data, size_t cnt, size_t size, const Comp_t cmp) {
    assert(data);
    assert(cmp);

    size_t p = partition(data, cnt, size, cmp);

    if (cnt > 2) {
        ON_sort(data, p + 1, size, cmp);
        ON_sort((char *) data + (p + 1) * size, cnt - (p + 1), size, cmp);
    }
}

#undef SWAP

