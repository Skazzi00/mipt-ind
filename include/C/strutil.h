#pragma once

typedef struct {
    size_t length;
    char *data;
} strView;

int strViewCmp(const void *a, const void *b);

int strViewCmpReversed(const void *a, const void *b);

size_t calcLines(const char *str);
