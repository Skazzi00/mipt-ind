#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "C/strutil.h"
#include "C/fileutil.h"

int main(int argc, const char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    errno = 0;
    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("fopen() failed");
        exit(EXIT_FAILURE);
    }
    errno = 0;
    fileDesc fileD = getFileDesc(file);
    if (errno) {
        perror("getFileDesc() failed");
        freeFileDesc(&fileD);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    if (fileD.lines) {
        qsort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmp);
    }
    puts("################SORTED################");
    for (size_t i = 0; i < fileD.linesCnt; ++i) {
        printf("%s\n", fileD.lines[i].data);
    }

    if (fileD.lines) {
        qsort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmpReversed);
    }
    puts("###########REVERSED#SORTED############");
    for (size_t i = 0; i < fileD.linesCnt; ++i) {
        printf("%s\n", fileD.lines[i].data);
    }

    puts("################SOURCE################");
    char *data = fileD.rawData + 1;
    for (size_t i = 0; i < fileD.linesCnt; ++i) {
        printf("%s\n", data);
        data += strlen(data) + 1;
    }

    freeFileDesc(&fileD);
    fclose(file);
}
