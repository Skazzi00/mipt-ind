#pragma once

#include <stddef.h>


typedef int (*Comp_t)(const void *a, const void *b);

typedef void (*Sort_t)(void *, size_t,size_t, Comp_t);

/*!
 * Sort array (quicksort implementation)
 * @param data pointer to array
 * @param cnt count of items
 * @param size size of one item
 * @param cmp comparator for comparing items
 */
void ON_sort(void *data, size_t cnt, size_t size, Comp_t cmp);
