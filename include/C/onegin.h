#pragma once

#include <stddef.h>


typedef int (*ON_CompType)(const void *a, const void *b);

void ON_sort(void *data, size_t cnt, size_t size, ON_CompType cmp);
