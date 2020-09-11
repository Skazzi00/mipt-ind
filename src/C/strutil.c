//
// Created by Alexandr Eremin on 11.09.2020.
//
#include <string.h>
#include <assert.h>

#include "C/strutil.h"

int strViewCmp(const void *a, const void *b) {
    const strView *ia = (const strView *) a;
    const strView *ib = (const strView *) b;
    return strcmp(ia->data, ib->data);
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