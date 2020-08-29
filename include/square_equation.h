#pragma once

#include <cmath>
#include <cassert>
#include <algorithm>

inline constexpr double EPS = 1e-6;

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
 * \details If root doesn't exist corresponding value is undefined. In case of two roots, first root will be less than second.
 */
inline SquareEquation::Solution solve(const SquareEquation &eq) noexcept {
    assert(std::isfinite(eq.a) && std::isfinite(eq.b) && std::isfinite(eq.c));

    auto const isZero = [](const double d) { return std::abs(d) < EPS; };

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
        auto mnmx = std::minmax((-eq.b - std::sqrt(d)) / (2. * eq.a), (-eq.b + std::sqrt(d)) / (2. * eq.a));
        return {
                SquareEquation::Solution::Two,
                mnmx.first,
                mnmx.second
        };
    }
}