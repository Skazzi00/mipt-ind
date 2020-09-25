#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

#include "C/strutil.h"
#include "C/fileutil.h"
#include "C/onegin.h"

#define HELP_STRING  "Usage: %s [-ho] <input>\n"         \
                     "Options:\n"                        \
                     "    -h print this help message\n"  \
                     "    -o flag for using onegin sort" \


static Sort_t sort_f = &qsort;

int parseOpts(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    if (argc < 2) {
        fprintf(stderr, HELP_STRING, argv[0]);
        return EXIT_FAILURE;
    }

    if (parseOpts(argc, argv) != 0) {
        return EXIT_FAILURE;
    }

    FILE *file = fopenChecked(argv[optind], "rb");
    if (!file) {
        return EXIT_FAILURE;
    }

    const fileMeta_t fileMeta = getFileMetaChecked(file);
    if (fileMeta.status == ERROR) {
        fclose(file);
        return EXIT_FAILURE;
    }

    sort_f(fileMeta.lines, fileMeta.linesCnt, sizeof(fileMeta.lines[0]), strViewCmp);
    puts("################SORTED################");
    printLines(fileMeta.lines, fileMeta.linesCnt);

    sort_f(fileMeta.lines, fileMeta.linesCnt, sizeof(fileMeta.lines[0]), strViewCmpReversed);
    puts("###########REVERSED#SORTED############");
    printLines(fileMeta.lines, fileMeta.linesCnt);


    puts("################SOURCE################");
    printRawData(fileMeta._rawData.data, fileMeta._rawData.length);

    freeFileMeta(&fileMeta);
    fclose(file);
}

int parseOpts(int argc, char **argv) {
    int opt = -1;
    while ((opt = getopt(argc, argv, "ho")) != -1) {
        switch (opt) {
            case 'h':
                fprintf(stdout, HELP_STRING, argv[0]);
                return EXIT_FAILURE;

            case 'o':
                sort_f = &ON_sort;
                break;

            default:
                fprintf(stderr, HELP_STRING, argv[0]);
                return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
