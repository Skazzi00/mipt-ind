#include <cfloat>
#include <cmath>
#include <cassert>
#include <stdexcept>
#include <iostream>

const int INF_ROOTS = -1;
const int NO_REAL_ROOTS = -2;

bool almostEqual(double first, double second) {
    return std::abs(second - first) < DBL_EPSILON;
}

bool isZero(double val) {
    return almostEqual(val, 0);
}

/*
 * Finds the real roots of a quadratic equation.
 *
 * @param a first coefficient
 * @param b second coefficient
 * @param c third coefficient
 * @param x1 reference where to write the first root
 * @param x2 reference where to write the second root
 *
 * @return number of roots or special code(INF_ROOTS, NO_REAL_ROOTS)
 *
 * If the root cannot be found, then nothing will be written to the corresponding reference
 */
int findRealRoots(double a, double b, double c, double &x1, double &x2) {
    assert(std::isfinite(a));
    assert(std::isfinite(b));
    assert(std::isfinite(c));

    assert(&x1 != &x2);

    if (isZero(a)) {
        if (isZero(b)) {
            if (isZero(c)) return INF_ROOTS;
            else return 0;
        } else {
            x1 = -c / b;
            return 1;
        }
    }
    double d = b * b - 4 * a * c;
    if (isZero(d)) {
        x1 = x2 = (-b) / (2 * a);
        return 1;
    }
    if (d < 0) {
        return NO_REAL_ROOTS;
    }
    double sqrtD = sqrt(d);

    x1 = (-b - sqrtD) / (2 * a);
    x2 = (-b + sqrtD) / (2 * a);

    return 2;
}

void solveEquation(std::istream &in, std::ostream &out) {
    out << "Enter equation coefficients:" << std::endl;
    double a, b, c;
    in >> a >> b >> c;
    double x1, x2;
    int numOfRoots = findRealRoots(a, b, c, x1, x2);
    switch (numOfRoots) {
        case 0:
            out << "No roots";
            break;
        case 1:
            out << "One root: x = " << x1;
            break;
        case 2:
            out << "Two roots: x1 = " << x1 << ", x2 = " << x2;
            break;
        case NO_REAL_ROOTS:
            out << "No real roots";
            break;
        case INF_ROOTS:
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
