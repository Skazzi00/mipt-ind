#include "gtest/gtest.h"

#include "square_equation.h"

namespace {
    using namespace se;

    TEST(SolveTest, InfinitySolutions) {
        EXPECT_EQ(SquareEquation::Solution::Inf, solve({0, 0, 0}).numberOfRoots);
    }

    TEST(SolveTest, ZeroSolutions) {
        EXPECT_EQ(SquareEquation::Solution::Zero, solve({0, 0, 1}).numberOfRoots);
        EXPECT_EQ(SquareEquation::Solution::Zero, solve({3, 0, 1}).numberOfRoots);
    }

    TEST(SolveTest, OneSolution) {
        auto const solution1 = solve({0, 1, 0});
        EXPECT_EQ(SquareEquation::Solution::One, solution1.numberOfRoots);
        EXPECT_DOUBLE_EQ(0., solution1.firstRoot);

        auto const solution2 = solve({0, 2, 1});
        EXPECT_EQ(SquareEquation::Solution::One, solution2.numberOfRoots);
        EXPECT_DOUBLE_EQ(-0.5, solution2.firstRoot);

        auto const solution3 = solve({3, 0, 0});
        EXPECT_EQ(SquareEquation::Solution::One, solution3.numberOfRoots);
        EXPECT_DOUBLE_EQ(-0., solution3.firstRoot);
    }

    TEST(SolveTest, TwoSolutions) {
        auto const solution = solve({1, 0, -4});
        EXPECT_EQ(SquareEquation::Solution::Two, solution.numberOfRoots);
        EXPECT_DOUBLE_EQ(-2., solution.firstRoot);
        EXPECT_DOUBLE_EQ(2., solution.secondRoot);
    }
}