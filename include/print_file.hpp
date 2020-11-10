#pragma once

#include <cstdio>

namespace mipt {
    template<typename T>
    inline void printToFile(FILE *file, T) {
        fprintf(file, "<VALUE>");
    }

    template<>
    inline void printToFile(FILE *file, int val) {
        fprintf(file, "%d", val);
    }

    template<>
    inline void printToFile(FILE *file, unsigned char val) {
        fprintf(file, "%d", val);
    }


    template<>
    inline void printToFile(FILE *file, double val) {
        fprintf(file, "%lg", val);
    }
}
