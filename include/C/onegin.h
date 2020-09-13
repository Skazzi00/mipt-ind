#pragma once

#include <stddef.h>


typedef int (*ON_CompType)(const void *a, const void *b);

/*!
 * Sort array (quicksort implementation)
 * @param data pointer to array
 * @param cnt count of items
 * @param size size of one item
 * @param cmp comparator for comparing items
 */
void ON_sort(void *data, size_t cnt, size_t size, ON_CompType cmp);
