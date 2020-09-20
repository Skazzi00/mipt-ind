#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "C/strutil.h"
#include "C/fileutil.h"


int main(int argc, const char **argv) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopenChecked(argv[1], "rb");
    if (!file) {
        return EXIT_FAILURE;
    }

    const fileDesc_t fileD = getFileDescChecked(file);
    if (fileD.status == ERROR) {
        fclose(file);
        return EXIT_FAILURE;
    }

    qsort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmp);
    puts("################SORTED################");
    printLines(fileD.lines, fileD.linesCnt);

    qsort(fileD.lines, fileD.linesCnt, sizeof(fileD.lines[0]), strViewCmpReversed);
    puts("###########REVERSED#SORTED############");
    printLines(fileD.lines, fileD.linesCnt);


    puts("################SOURCE################");
    printRawData(fileD._rawData.data, fileD._rawData.length);

    freeFileDesc(&fileD);
    fclose(file);
}
