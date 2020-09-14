//
// Created by Alexandr Eremin on 11.09.2020.
//
#include <assert.h>
#include <ctype.h>

#ifdef _WIN32
#include <winsock.h>
#else

#include <arpa/inet.h>

#endif

#include "C/strutil.h"


static int smartStrViewCmp(const strView *sv1, const strView *sv2, int delta) {
    assert(sv1);
    assert(sv2);
    assert(delta == 1 || delta == -1);
    const char *s1 = sv1->data;
    const char *s2 = sv2->data;
    int i = 0;
    int j = 0;
    if (delta < 0) {
        i = (int) sv1->length - 1;
        j = (int) sv2->length - 1;
    }
    while (ispunct(s1[i]) || isspace(s1[i])) i += delta;
    while (ispunct(s2[j]) || isspace(s2[j])) j += delta;

    while (1) {
        while (ispunct(s1[i])) i += delta;
        while (ispunct(s2[j])) j += delta;
        if (delta < 0 && s1[i] < 0 && s2[j] < 0) {
            const unsigned short c1 = ntohs(*((unsigned short *) (s1 + i + delta)));
            const unsigned short c2 = ntohs(*((unsigned short *) (s2 + j + delta)));
            if (c1 != c2) { // NOLINT
                return c1 < c2 ? -1 : 1;
            }
            i += delta * 2;
            j += delta * 2;
        } else {
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
