#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "C/oneginC.h"


static inline void swap(void *const a, void *const b, size_t size) {
    assert(a);
    assert(b);
    void *tmp = malloc(size);
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
    free(tmp);
}

static size_t partition(void *const data, size_t cnt, size_t size, const ON_CompType cmp) {
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
        swap((char *) data + first++ * size, (char *) data + last-- * size, size);
    }
    free(val);
    return last;
}

void ON_sort(void *const data, size_t cnt, size_t size, const ON_CompType cmp) {
    assert(data);
    assert(cmp);
    size_t p = partition(data, cnt, size, cmp);
    if (cnt > 2) {
        ON_sort(data, p + 1, size, cmp);
        ON_sort((char *) data + (p + 1) * size, cnt - (p + 1), size, cmp);
    }
}

