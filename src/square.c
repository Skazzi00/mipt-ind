#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "square_equationC.h"

#define INPUT_SIZE 256

int main() {
    SquareEquation equation;
    puts("Enter square equation coefficients:");

    char input[INPUT_SIZE]; // in ideal world there should be dynamic allocation
    fgets(input, INPUT_SIZE, stdin);
    int good = sscanf(input, "%lf%lf%lf", &equation.a, &equation.b, &equation.c);
    if (!good) {
        puts("Wrong input! Restart program");
        exit(1);
    }

    Solution solution = solve(equation);
    switch (solution.type) {
        case ZERO:
            puts("No solutions");
            break;
        case ONE:
            printf("One solution: x = %lg", solution.firstRoot);
            break;
        case TWO:
            printf("Two solutions: x1 = %lg, x2 = %lg", solution.firstRoot, solution.secondRoot);
            break;
        case INF:
            puts("Infinity solution");
            break;
        default:
            assert(!"Something gone wrong");
    }
}