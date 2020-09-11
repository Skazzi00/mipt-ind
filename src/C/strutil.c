//
// Created by Alexandr Eremin on 11.09.2020.
//
#include <string.h>
#include <assert.h>

#include "C/strutil.h"

int cstring_cmp(const void *a, const void *b) {
    const char **ia = (const char **) a;
    const char **ib = (const char **) b;
    return strcmp(*ia, *ib);
}

size_t calcLines(const char * str) {
    assert(str);
    size_t lines = 1;
    while (*str) {
        if (*str == '\n') ++lines;
        ++str;
    }
    return lines;
}