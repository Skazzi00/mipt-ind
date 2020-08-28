#pragma once

#include <cmath>
#include <cassert>

struct SquareEquation {
    double a;
    double b;
    double c;

    struct Solution {

        enum {
            Inf = -1,
            Zero,
            One,
            Two
        } numberOfRoots;
        double firstRoot;
        double secondRoot;
    };
};


/**
 * \brief Finds the real roots of a quadratic equation.
 *
 * \param[in] eq equation to be solved
 *
 * \return solution of equation represented by SquareEquation::Solution
 *
 * \details If root doesn't exist corresponding value is undefined. In case of two roots, first root will be less then second.
 */
SquareEquation::Solution solve(const SquareEquation &eq) {
    assert(std::isfinite(eq.a) && std::isfinite(eq.b) && std::isfinite(eq.c));

    auto const isZero = [](const double d) {return d == 0.;};

    if (isZero(eq.a)) {
        if (isZero(eq.b)) {
            return {
                    isZero(eq.c) ? SquareEquation::Solution::Inf : SquareEquation::Solution::Zero,
                    0.,
                    0.
            };
        } else {
            return {
                    SquareEquation::Solution::One,
                    -eq.c / eq.b,
                    0.
            };
        }
    }
    const double d = eq.b * eq.b - 4. * eq.a * eq.c;
    if (isZero(d)) {
        return {
                SquareEquation::Solution::One,
                -eq.b / (2. * eq.a),
                0.
        };
    } else if (d < 0) {
        return {SquareEquation::Solution::Zero, 0., 0.};
    } else {
        return {
                SquareEquation::Solution::Two,
                (-eq.b - std::sqrt(d)) / (2. * eq.a),
                (-eq.b + std::sqrt(d)) / (2. * eq.a)
        };
    }
}