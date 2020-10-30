#pragma once

#include "strutil.h"

enum STATUS_CODE {
    OK = 0, ERROR = 1
};

typedef enum STATUS_CODE STATUS_CODE_E;

typedef struct {
    size_t        linesCnt;
    strView_t     *lines;
    str_t         _rawData;
    STATUS_CODE_E status;
}                        fileMeta_t;

/*!
 * Read lines from UTF-8 file
 * @param file pointer to FILE
 * @return file descriptor containing lines count, lines as an array strView, and pointer to raw data from file
 */
fileMeta_t getFileMeta(FILE *file);

/*!
 * Free all resource from fileDesc
 * @param fileD pointer to structure to release
 */
void freeFileMeta(const fileMeta_t *fileD);

/*!
 * fopen() with error handling
 * @param filename
 * @param mode
 * @return Opened FILE or NULL if error happened
 */
FILE *fopenChecked(const char *filename, const char *mode);

/*!
 * getFileMeta() with error handling
 */
fileMeta_t getFileMetaChecked(FILE *file);

long getFileSize(FILE *file);

