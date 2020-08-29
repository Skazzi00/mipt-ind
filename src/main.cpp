#include <stdexcept>
#include <iostream>

#include "square_equation.h"

namespace {
    void solveEquation(std::istream &in, std::ostream &out) {
        out << "Enter equation coefficients:" << std::endl;
        SquareEquation eq{};
        [[maybe_unused]] bool const good = static_cast<bool>(in >> eq.a >> eq.b >> eq.c);
        assert(good);
        auto const[nRoots, firstRoot, secondRoot] = solve(eq);
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
}

int main() {
    std::cout << "Program for solve quadratic equation" << std::endl;

    solveEquation(std::cin, std::cout);

    return 0;
}
