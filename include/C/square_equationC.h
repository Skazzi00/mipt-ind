#pragma once

#include <assert.h>
#include <math.h>
#include <stdio.h>

#define EPS 1e-6

enum SolutionType {
    ZERO,
    ONE,
    TWO,
    INF = -1,
};

typedef struct {
    enum SolutionType type;
    double firstRoot;
    double secondRoot;
} Solution;

typedef struct {
    double a;
    double b;
    double c;
} SquareEquation;

static inline int isValid(SquareEquation const eq) {
    return isfinite(eq.a) && isfinite(eq.b) && isfinite(eq.c);
}

static inline int isZero(double const d) {
    return fabs(d) < EPS;
}

/**
 * \brief Finds the real roots of a quadratic equation.
 *
 * \param[in] eq equation to be solved
 *
 * \return solution of equation represented by SquareEquation::Solution
 *
 * \details If root doesn't exist corresponding value is undefined. In case of two roots, first root will be less than second.
 */
static inline Solution solve(SquareEquation const eq) {
    assert(isValid(eq));
    Solution solution = {ZERO, 0., 0.};
    if (isZero(eq.a)) {
        if (isZero(eq.b)) {
            if (isZero(eq.c))
                solution.type = INF;
        } else {
            solution.type = ONE;
            solution.firstRoot = -eq.c / eq.b;
        }
    } else {
        double const det = eq.b * eq.b - 4. * eq.a * eq.c;
        if (isZero(det)) {
            solution.type = ONE;
            solution.firstRoot = -eq.b / (2. * eq.a);
        } else if (det > 0.) {
            solution.type = TWO;
            solution.firstRoot = (-eq.b - sqrt(det)) / (2. * eq.a);
            solution.secondRoot = (-eq.b + sqrt(det)) / (2. * eq.a);
        }
    }
    return solution;
}

