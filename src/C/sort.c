#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

#include "C/strutil.h"
#include "C/fileutil.h"

void printLines(const strView_t *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        printf("%s\n", data[i].data);
    }
}

int main(int argc, const char **argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
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
    const fileDesc_t fileD = getFileDesc(file);
    if (errno != 0 || !fileD.lines) {
        perror("getFileDesc() failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    qsort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmp);
    puts("################SORTED################");
    printLines(fileD.lines, fileD.linesCnt);

    qsort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmpReversed);
    puts("###########REVERSED#SORTED############");
    printLines(fileD.lines, fileD.linesCnt);


    puts("################SOURCE################");
    const char *data = fileD._rawData.data;
    for (size_t i = 1; i < fileD._rawData.length; ++i) {
        if (data[i] == '\0') {
            putchar('\n');
        } else {
            putchar(data[i]);
        }
    }

    freeFileDesc(&fileD);
    fclose(file);
}
