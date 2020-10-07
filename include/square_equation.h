#pragma once

enum SolutionType {
    ZERO_SOLUTION,
    ONE_SOLUTION,
    TWO_SOLUTION,
    INF_SOLUTION = -1,
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

/**
 * \brief Finds the real roots of a quadratic equation.
 *
 * \param[in] eq equation to be solved
 *
 * \return solution of equation represented by SquareEquation::Solution
 *
 * \details If root doesn't exist corresponding value is undefined. In case of two roots, first root will be less than second.
 */
Solution solve(SquareEquation const eq);
