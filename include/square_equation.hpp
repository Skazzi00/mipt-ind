#pragma once

#include "../../../../Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/cmath"

#include "../../../../Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/cassert"

namespace se {

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


    inline bool isValid(const SquareEquation eq) noexcept {
        return std::isfinite(eq.a) && std::isfinite(eq.b) && std::isfinite(eq.c);
    }

/**
 * \brief Finds the real roots of a quadratic equation.
 *
 * \param[in] eq equation to be solved
 *
 * \return Solution of equation represented by SquareEquation::Solution
 *
 * \details If root doesn't exist corresponding value is undefined.
 */
    inline SquareEquation::Solution solve(const SquareEquation eq) noexcept {
        assert(isValid(eq));

        auto const isZero = +[](const double d) { return std::abs(d) < EPS; };

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
            return {
                    SquareEquation::Solution::Zero, 0., 0.
            };
        } else {
            return {
                    SquareEquation::Solution::Two,
                    (-eq.b - std::sqrt(d)) / (2. * eq.a),
                    (-eq.b + std::sqrt(d)) / (2. * eq.a)
            };
        }
    }
}