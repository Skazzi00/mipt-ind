#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "C/square_equation.h"

#define INPUT_SIZE 256

int main() {
    SquareEquation equation;
    puts("Enter square equation coefficients:");

    char input[INPUT_SIZE]; // in ideal world there should be dynamic allocation
    fgets(input, INPUT_SIZE, stdin);
    int good = sscanf(input, "%lf%lf%lf", &equation.a, &equation.b, &equation.c);
    if (good != 3) {
        puts("Wrong input! Restart program");
        exit(1);
    }

    Solution solution = solve(equation);
    switch (solution.type) {
        case ZERO_SOLUTION:
            puts("No solutions");
            break;
        case ONE_SOLUTION:
            printf("One solution: x = %lg", solution.firstRoot);
            break;
        case TWO_SOLUTION:
            printf("Two solutions: x1 = %lg, x2 = %lg", solution.firstRoot, solution.secondRoot);
            break;
        case INF_SOLUTION:
            puts("Infinity solution");
            break;
        default:
            assert(0 && "Something gone wrong");
    }
}
