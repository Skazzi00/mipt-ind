#include <cmath>
#include <cassert>
#include <stdexcept>
#include <iostream>

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

bool isZero(const double d) {
    return d == 0.;
}

SquareEquation::Solution solve(const SquareEquation &eq) {
    assert(std::isfinite(eq.a));
    assert(std::isfinite(eq.b));
    assert(std::isfinite(eq.c));


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

void solveEquation(std::istream &in, std::ostream &out) {
    out << "Enter equation coefficients:" << std::endl;
    SquareEquation eq{};
    [[maybe_unused]] bool const good = static_cast<bool>(in >> eq.a >> eq.b >> eq.c);
    assert(good);
    auto const [nRoots, firstRoot, secondRoot] = solve(eq);
    switch (nRoots) {
        case SquareEquation::Solution::Zero:
            out << "No roots";
            break;
        case SquareEquation::Solution::One:
            out << "One root: x = " << firstRoot;
            break;
        case SquareEquation::Solution::Two:
            out << "Two roots: x1 = " << firstRoot << ", x2 = " << secondRoot;
            break;
        case SquareEquation::Solution::Inf:
            out << "Infinity number of roots";
            break;
        default:
            throw std::runtime_error("findRealRoots return undefined code");
    }
    out << std::endl;
}

int main() {
    std::cout << "Program for solve quadratic equation" << std::endl;

    solveEquation(std::cin, std::cout);

    return 0;
}
