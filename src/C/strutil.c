//
// Created by Alexandr Eremin on 11.09.2020.
//
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "C/strutil.h"


static int smartStrViewCmp(const strView *sv1, const strView *sv2, int delta) {
    const char *s1 = sv1->data;
    const char *s2 = sv2->data;
    int i = 0;
    int j = 0;
    if (delta < 0) {
        i = (int) sv1->length - 1;
        j = (int) sv2->length - 1;
    }
    while (1) {

        while (ispunct(s1[i])) i += delta;
        while (ispunct(s2[j])) j += delta;
        if (s1[i] != s2[j]) {
            return s1[i] < s2[j] ? -1 : 1;
        }

        if (s1[i] == '\0') {
            return 0;
        }
        if (isspace(s1[i])) {
            while (isspace(s1[i])) i += delta;
            while (isspace(s2[j])) j += delta;
        } else {
            i += delta;
            j += delta;
        }
    }
}

int strViewCmp(const void *a, const void *b) {
    assert(a);
    assert(b);
    const strView *ia = (const strView *) a;
    const strView *ib = (const strView *) b;
    return smartStrViewCmp(ia, ib, 1);
}

int strViewCmpReversed(const void *a, const void *b) {
    assert(a);
    assert(b);
    const strView *ia = (const strView *) a;
    const strView *ib = (const strView *) b;
    return smartStrViewCmp(ia, ib, -1);
}

size_t calcLines(const char *str) {
    assert(str);
    size_t lines = 1;
    while (*str) {
        if (*str == '\n') ++lines;
        ++str;
    }
    return lines;
}