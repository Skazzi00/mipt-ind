#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "C/oneginC.h"
#include "C/strutil.h"
#include "C/fileutil.h"

int main(int argc, const char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: program filename\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "rb");
    fileDesc fileD = getFileDesc(file);
    if (fileD.lines) {
        ON_sort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmp);
    }
    for (size_t i = 0; i < fileD.linesCnt; ++i) {
        printf("%s\n", fileD.lines[i].data);
    }
    freeFileDesc(&fileD);
    fclose(file);
}
/*
 * format file before program
 * punctuation and merge spaces
 * reverse comparator
 */