#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "C/fileutil.h"
#include "C/strutil.h"

static inline long getFileSize(FILE *file) {
    errno = 0;
    long pos = ftell(file);
    if (errno) { return -1; }

    if (fseek(file, 0, SEEK_END)) { return -1; }

    long length = ftell(file) + 1;
    if (errno) { return -1; }

    if (fseek(file, pos, SEEK_SET)) { return -1; }
    return length;
}

static inline char *readData(FILE *file, size_t length) {
    char *dataPtr = malloc(length + 1);
    if (!dataPtr) {
        return NULL;
    }
    dataPtr[0] = '\0';
    fread(dataPtr + 1, sizeof(dataPtr[0]), length, file);
    if (ferror(file)) {
        free(dataPtr);
        return NULL;
    }
    return dataPtr;
}

static inline strView *dataToLinesArray(char *dataPtr, size_t linesCnt) {
    strView *result = calloc(linesCnt, sizeof(result[0]));
    if (!result) {
        return NULL;
    }
    size_t curLine = 0;
    result[curLine].length = 0;
    result[curLine].data = dataPtr;
    while (*dataPtr) {
        if (*dataPtr == '\n') {
            *dataPtr = '\0';
            result[++curLine].data = dataPtr + 1;
            result[curLine].length = 0;
        } else {
            result[curLine].length++;
        }
        dataPtr++;
    }
    return result;
}

fileDesc getFileDesc(FILE *file) {
    assert(file);
    fileDesc result = {0, NULL, NULL};

    size_t length = (size_t) getFileSize(file);
    if (errno) {
        return result;
    }

    result.rawData = readData(file, length);
    if (!result.rawData) {
        return result;
    }
    char *dataPtr = result.rawData + 1;

    result.linesCnt = calcLines(dataPtr);
    result.lines = dataToLinesArray(dataPtr, result.linesCnt);
    if (!result.linesCnt) {
        free(result.rawData);
        return result;
    }
    return result;
}

void freeFileDesc(const fileDesc *fileD) {
    if (!fileD) return;
    if (fileD->lines) free(fileD->lines);
    if (fileD->rawData) free(fileD->rawData);
}
