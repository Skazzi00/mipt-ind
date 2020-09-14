#pragma once

#include "strutil.h"

typedef struct {
    size_t linesCnt;
    strView *lines;
    char *_rawData;
} fileDesc;

/*!
 * Read lines from UTF-8 file
 * @param file pointer to FILE
 * @return file descriptor containing lines count, lines as an array strView, and pointer to raw data from file
 */
fileDesc getFileDesc(FILE *file);

/*!
 * Free all resource from fileDesc
 * @param fileD pointer to structure to release
 */
void freeFileDesc(const fileDesc *fileD);
