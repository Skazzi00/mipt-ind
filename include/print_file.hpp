#pragma once

#include <cstdio>

namespace mipt {
    template<typename T>
    void printToFile(FILE *file, T) {
        fprintf(file, "<VALUE>");
    }

    template<>
    void printToFile(FILE *file, int val) {
        fprintf(file, "%d", val);
    }

    template<>
    void printToFile(FILE *file, unsigned char val) {
        fprintf(file, "%d", val);
    }


    template<>
    void printToFile(FILE *file, double val) {
        fprintf(file, "%lg", val);
    }
}
