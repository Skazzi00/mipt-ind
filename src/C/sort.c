#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "C/oneginC.h"

#define START_CAPACCITY 8

typedef struct {
    size_t linesCnt;
    char **lines;
    char *rawData;
} fileDesc;

int cstring_cmp(const void *a, const void *b);

fileDesc getFileDesc(FILE *file);

void freeFileDesc(fileDesc *fileD);

int main(int argc, const char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: program filename\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "rb");
    fileDesc fileD = getFileDesc(file);
    if (fileD.lines) {
        ON_sort(fileD.lines, fileD.linesCnt, sizeof(char *), cstring_cmp);
    }
    for (size_t i = 0; i < fileD.linesCnt; ++i) {
        printf("%s\n", fileD.lines[i]);
    }
    freeFileDesc(&fileD);
    fclose(file);
}

int cstring_cmp(const void *a, const void *b) {
    const char **ia = (const char **) a;
    const char **ib = (const char **) b;
    return strcmp(*ia, *ib);
}


size_t getFileSize(FILE *file) {
    size_t pos = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file) + 1;
    fseek(file, pos, SEEK_SET);
    return length;
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

fileDesc getFileDesc(FILE *file) {
    assert(file);
    fileDesc result = {0, NULL, NULL};
    size_t length = getFileSize(file);
    char *dataPtr = malloc(length);
    if (!dataPtr) {
        fprintf(stderr, "File is to big\n");
        fclose(file);
        exit(1);
    }
    result.rawData = dataPtr;
    fread(dataPtr, sizeof(dataPtr[0]), length, file);
    result.linesCnt = calcLines(dataPtr);
    result.lines = calloc(result.linesCnt, sizeof(result.lines));
    size_t curLine = 0;
    result.lines[curLine++] = dataPtr;
    while (*dataPtr) {
        if (*dataPtr == '\n') {
            *dataPtr = '\0';
            result.lines[curLine++] = dataPtr + 1;
        }
        dataPtr++;
    }
    return result;
}

void freeFileDesc(fileDesc *fileD) {
    if (!fileD) return;
    if (fileD->lines) free(fileD->lines);
    if (fileD->rawData) free(fileD->rawData);
}
/*
 * format file before program
 * punctuation and merge spaces
 * reverse comporator
 * string view
 */