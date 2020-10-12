#include <assert.h>
#include <math.h>

#ifdef CPP
#include <cmath>
#endif
#include "square_equation.h"

#define EPS 1e-6

static inline int isValid(SquareEquation const eq) {
    return isfinite(eq.a) && isfinite(eq.b) && isfinite(eq.c);
}

static inline int isZero(double const d) {
    return fabs(d) < EPS;
}

Solution solve(SquareEquation const eq) {
    assert(isValid(eq));

    Solution solution = {ZERO_SOLUTION, 0., 0.};
    if (isZero(eq.a)) {
        if (isZero(eq.b)) {
            if (isZero(eq.c))
                solution.type = INF_SOLUTION;
        } else {
            solution.type = ONE_SOLUTION;
            solution.firstRoot = -eq.c / eq.b;
        }
    } else {
        double const det = eq.b * eq.b - 4. * eq.a * eq.c;
        if (isZero(det)) {
            solution.type = ONE_SOLUTION;
            solution.firstRoot = -eq.b / (2. * eq.a);
        } else if (det > 0.) {
            solution.type = TWO_SOLUTION;
            solution.firstRoot = (-eq.b - sqrt(det)) / (2. * eq.a);
            solution.secondRoot = (-eq.b + sqrt(det)) / (2. * eq.a);
        }
    }
    return solution;
}

