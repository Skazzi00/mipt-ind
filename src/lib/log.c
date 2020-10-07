#include <stdio.h>

#include "log.h"

FILE *logfile;

void initLog(FILE* file) {
    logfile = file;
}
