#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "C/fileutil.h"
#include "C/strutil.h"

size_t getFileSize(FILE *file) {
    size_t pos = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file) + 1;
    fseek(file, pos, SEEK_SET);
    return length;
}

fileDesc getFileDesc(FILE *file) {
    assert(file);
    fileDesc result = {0, NULL, NULL};

    size_t length = getFileSize(file);
    if (errno) {
        return result;
    }

    char *dataPtr = malloc(length + 1); // +1 for \0 at the begining
    if (!dataPtr) {
        return result;
    }
    result.rawData = dataPtr;
    dataPtr[0] = '\0';
    dataPtr++;
    fread(dataPtr, sizeof(dataPtr[0]), length, file);
    if (ferror(file)) {
        return result;
    }

    result.linesCnt = calcLines(dataPtr);
    result.lines = calloc(result.linesCnt, sizeof(result.lines[0]));
    if (!result.lines) {
        return result;
    }

    size_t curLine = 0;
    result.lines[curLine].length = 0;
    result.lines[curLine].data = dataPtr;
    while (*dataPtr) {
        if (*dataPtr == '\n') {
            *dataPtr = '\0';
            result.lines[++curLine].data = dataPtr + 1;
            result.lines[curLine].length = 0;
        } else {
            result.lines[curLine].length++;
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