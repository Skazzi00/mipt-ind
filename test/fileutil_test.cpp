#include "gtest/gtest.h"

extern "C" {
#include "fileutil.h"
}

namespace {
    TEST(GetFileDescTest, empty) {
        FILE *tmpf = tmpfile();
        fileMeta_t desc = getFileMeta(tmpf);
        ASSERT_EQ(1, desc.linesCnt);
        ASSERT_EQ(0, desc.lines[0].length);
        ASSERT_EQ(0, strcmp("", desc.lines[0].data));
        freeFileMeta(&desc);
    }

    TEST(GetFileDescTest, one_char) {
        FILE *tmpf = tmpfile();
        fputc('a', tmpf);
        fseek(tmpf, 0, SEEK_SET);
        fileMeta_t desc = getFileMeta(tmpf);
        ASSERT_EQ(1, desc.linesCnt);
        ASSERT_EQ(1, desc.lines[0].length);
        ASSERT_EQ(0, strcmp("a", desc.lines[0].data));
        freeFileMeta(&desc);
    }

    TEST(GetFileDescTest, sample_string) {
        const std::string sample = "Hello, World!";
        FILE *tmpf = tmpfile();
        fputs(sample.c_str(), tmpf);
        fseek(tmpf, 0, SEEK_SET);
        fileMeta_t desc = getFileMeta(tmpf);
        ASSERT_EQ(1, desc.linesCnt);
        ASSERT_EQ(sample.length(), desc.lines[0].length);
        ASSERT_EQ(0, strcmp(sample.c_str(), desc.lines[0].data));
        freeFileMeta(&desc);
    }
}