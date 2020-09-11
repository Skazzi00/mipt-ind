#pragma once

typedef struct {
    size_t linesCnt;
    char **lines;
    char *rawData;
} fileDesc;

fileDesc getFileDesc(FILE *file);

void freeFileDesc(fileDesc *fileD);

size_t getFileSize(FILE *file);