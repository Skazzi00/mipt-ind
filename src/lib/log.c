#include <stdio.h>

#include "log.h"

FILE *logfile = NULL;

void initLog(FILE* file) {
    logfile = file;
}
