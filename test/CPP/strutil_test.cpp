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

    TEST(StrViewCmp, empty) {
        strView_t a{0, ""};
        strView_t b{0, ""};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));
    }

    TEST(StrViewCmp, equalStrings) {
        strView_t a{3, "abc"};
        strView_t b{3, "abc"};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));

    }

    TEST(strViewCmp, differentStrings) {
        strView_t a{7, "abcdafg"};
        strView_t b{6, "abcdef"};
        ASSERT_TRUE(strViewCmp(&a, &b) < 0);
        ASSERT_TRUE(strViewCmp(&b, &a) > 0);
    }

    TEST(StrViewCmpReversed, empty) {
        strView_t a{0, ""};
        strView_t b{0, ""};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));
    }

    TEST(StrViewCmpReversed, equalStrings) {
        strView_t a{3, "abc"};
        strView_t b{3, "abc"};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));

    }

    TEST(strViewCmpReversed, differentStrings) {
        strView_t a{8, "abcdafff"};
        strView_t b{6, "cdefff"};
        ASSERT_TRUE(strViewCmp(&a, &b) < 0);
        ASSERT_TRUE(strViewCmp(&b, &a) > 0);
    }
}