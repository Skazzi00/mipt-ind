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
        strView a{0, ""};
        strView b{0, ""};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));
    }

    TEST(StrViewCmp, equalStrings) {
        strView a{3, "abc"};
        strView b{3, "abc"};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));

    }

    TEST(strViewCmp, differentStrings) {
        strView a{7, "abcdafg"};
        strView b{6, "abcdef"};
        ASSERT_TRUE(strViewCmp(&a, &b) < 0);
        ASSERT_TRUE(strViewCmp(&b, &a) > 0);
    }

    TEST(StrViewCmpReversed, empty) {
        strView a{0, ""};
        strView b{0, ""};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));
    }

    TEST(StrViewCmpReversed, equalStrings) {
        strView a{3, "abc"};
        strView b{3, "abc"};
        ASSERT_EQ(0, strViewCmp(&a, &b));
        ASSERT_EQ(0, strViewCmp(&b, &a));

    }

    TEST(strViewCmpReversed, differentStrings) {
        strView a{8, "abcdafff"};
        strView b{6, "cdefff"};
        ASSERT_TRUE(strViewCmp(&a, &b) < 0);
        ASSERT_TRUE(strViewCmp(&b, &a) > 0);
    }
}