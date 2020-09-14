#include "gtest/gtest.h"

extern "C" {
#include "C/strutil.h"
}

namespace {
    TEST(CalcLinesTest, empty) {
        ASSERT_EQ(1, calcLines(""));
    }

    TEST(CalcLinesTest, one_line) {
        ASSERT_EQ(1, calcLines("sample"));
    }

    TEST(CalcLinesTest, emptyLines) {
        ASSERT_EQ(4, calcLines("\n\n\n"));
    }

    TEST(CalcLinesTest, FourLines) {
        ASSERT_EQ(4, calcLines("abc\ndef\nghd\ndfas"));
    }
}