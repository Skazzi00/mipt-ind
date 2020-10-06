#include <stdio.h>

#include "C/log.h"

FILE *logfile;

void initLog(FILE* file) {
    logfile = file;
}
