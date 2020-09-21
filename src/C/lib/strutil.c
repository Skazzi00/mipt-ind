//
// Created by Alexandr Eremin on 11.09.2020.
//
#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#ifdef _WIN32
#include <winsock.h>
#else

#include <arpa/inet.h>

#endif

#include "C/strutil.h"


static int smartStrViewCmp(const char *s1, const char *s2, int delta) {
    assert(s1);
    assert(s2);
    assert(delta == 1 || delta == -1);

    while (ispunct(*s1) || isspace(*s1)) s1 += delta;
    while (ispunct(*s2) || isspace(*s2)) s2 += delta;

    while (1) {
        while (ispunct(*s1)) s1 += delta;
        while (ispunct(*s2)) s2 += delta;

        if (delta < 0 && *s1 < 0 && *s2 < 0) {
            const unsigned short c1 = ntohs(*((unsigned short *) (s1 + delta)));
            const unsigned short c2 = ntohs(*((unsigned short *) (s2 + delta)));

            if (c1 != c2) { // NOLINT
                return c1 < c2 ? -1 : 1;
            }

            s1 += delta * 2;
            s2 += delta * 2;
        } else {
            if (*s1 != *s2) {
                return *s1 < *s2 ? -1 : 1;
            }

            if (*s1 == '\0') {
                return 0;
            }

            if (isspace(*s1)) {
                while (isspace(*s1)) s1 += delta;
                while (isspace(*s2)) s2 += delta;
            } else {
                s1 += delta;
                s2 += delta;
            }
        }
    }
}

int strViewCmp(const void *a, const void *b) {
    assert(a);
    assert(b);

    const strView_t *ia = (const strView_t *) a;
    const strView_t *ib = (const strView_t *) b;

    return smartStrViewCmp(ia->data, ib->data, 1);
}

int strViewCmpReversed(const void *a, const void *b) {
    assert(a);
    assert(b);

    const strView_t *ia = (const strView_t *) a;
    const strView_t *ib = (const strView_t *) b;

    return smartStrViewCmp(ia->data + ia->length - 1, ib->data + ib->length - 1, -1);
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

void printLines(const strView_t *data, size_t length) {
    assert(data);

    for (size_t i = 0; i < length; ++i) {
        assert(data[i].data);
        printf("%s\n", data[i].data);
    }
}

void printRawData(const char *data, size_t length) {
    assert(data);

    for (size_t i = 1; i < length; ++i) {
        if (data[i] == '\0') {
            putchar('\n');
        } else {
            putchar(data[i]);
        }
    }
}
