#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "oneginC.h"

typedef struct {
    size_t linesCnt;
    size_t *linesSize;
} fileMD;

int cstring_cmp(const void *a, const void *b);

fileMD getFileMD(FILE *file);

int main(int argc, const char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: program filename\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    fileMD fileMd = getFileMD(file);
    char **lines = (char **) calloc(fileMd.linesCnt, sizeof(char *));
    for (size_t i = 0; i < fileMd.linesCnt; ++i) {
        lines[i] = (char *) malloc(fileMd.linesSize[i]);
        fgets(lines[i], (int) fileMd.linesSize[i], file);
    }
    lines[fileMd.linesCnt - 1][fileMd.linesSize[fileMd.linesCnt - 1] - 2] = '\n'; // for last line without \n

    ON_sort(lines, fileMd.linesCnt, sizeof(char *), cstring_cmp);
    for (size_t i = 0; i < fileMd.linesCnt; ++i) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    free(lines);
    free(fileMd.linesSize);
    fclose(file);
}

int cstring_cmp(const void *a, const void *b) {
    const char **ia = (const char **) a;
    const char **ib = (const char **) b;
    return strcmp(*ia, *ib);
}

fileMD getFileMD(FILE *file) {
    fileMD result = {0, NULL};
    int c;
    while (!feof(file)) {
        c = fgetc(file);
        if (c == '\n') {
            ++result.linesCnt;
        }
    }
    ++result.linesCnt;
    fseek(file, 0, SEEK_SET);
    result.linesSize = (size_t *) calloc(result.linesCnt, sizeof(size_t));
    int curLine = 0;
    while (!feof(file)) {
        c = fgetc(file);
        ++result.linesSize[curLine];
        if (c == '\n') {
            result.linesSize[curLine++] += 1;
        }
    }
    result.linesSize[curLine]++;
    fseek(file, 0, SEEK_SET);
    return result;
}