#pragma once

#include <cstdio>

namespace mipt {
    template<typename T>
    void printToFile(FILE *file, T val);

    template<>
    void printToFile(FILE *file, int val) {
        fprintf(file, "%d", val);
    }

    template<>
    void printToFile(FILE *file, double val) {
        fprintf(file, "%lg", val);
    }
}
