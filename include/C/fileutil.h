#pragma once

#include "strutil.h"

typedef struct {
    size_t linesCnt;
    strView *lines;
    char *rawData;
} fileDesc;

fileDesc getFileDesc(FILE *file);

void freeFileDesc(const fileDesc *fileD);
