#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "C/fileutil.h"
#include "C/strutil.h"

static inline long getFileSize(FILE *file) {
    assert(file);
    errno = 0;
    long pos = ftell(file);
    if (errno != 0) { return -1; }

    if (fseek(file, 0, SEEK_END)) { return -1; }

    long length = ftell(file) + 1;
    if (errno != 0) { return -1; }

    if (fseek(file, pos, SEEK_SET)) { return -1; }
    return length;
}

static inline char *readData(FILE *file, size_t length) {
    assert(file);
    char *dataPtr = calloc(length + 1, sizeof(dataPtr[0]));
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
    assert(dataPtr);
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
    if (errno != 0) {
        return result;
    }

    result._rawData = readData(file, length);
    if (!result._rawData) {
        return result;
    }
    char *dataPtr = result._rawData + 1;

    result.linesCnt = calcLines(dataPtr);
    result.lines = dataToLinesArray(dataPtr, result.linesCnt);
    if (!result.linesCnt) {
        free(result._rawData);
        return result;
    }
    return result;
}

void freeFileDesc(const fileDesc *fileD) {
    if (!fileD) return;
    if (fileD->lines) free(fileD->lines);
    if (fileD->_rawData) free(fileD->_rawData);
}
