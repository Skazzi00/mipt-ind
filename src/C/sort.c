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
    FILE *file = fopen(argv[1], "r");
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

fileDesc getFileDesc(FILE *file) {
    assert(file);
    fileDesc result = {0, NULL, NULL};
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file);
    if (length == 0) {
        return result;
    }
    fseek(file, 0, SEEK_SET);
    char *data = malloc(length);
    if (!data) {
        fprintf(stderr, "File is to big\n");
        fclose(file);
        exit(1);
    }
    result.rawData = data;
    fread(data, sizeof(char), length, file);
    result.linesCnt = 1;
    size_t curCapacity = START_CAPACCITY;
    result.lines = calloc(curCapacity, sizeof(result.lines));
    result.lines[0] = data;
    while (*data) {
        if (*data == '\n') {
            *data = '\0';
            if (result.linesCnt == curCapacity) {
                curCapacity *= 2;
                result.lines = realloc(result.lines, curCapacity * sizeof(result.lines));
            }
            result.lines[result.linesCnt++] = data + 1;
        }
        ++data;
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
 *
 */