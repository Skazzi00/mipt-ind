#pragma once

#include <cstring>
#include <string>

namespace mipt {
    struct string_view {
        using iterator = char *;
        using const_iterator = const char *;

        string_view(const char * data, size_t len) : mData(data), mLength(len) {}
        string_view(const char * data) : mData(data), mLength(data ? strlen(data) : 0) {}
        string_view(const std::string & data) : mData(data.c_str()), mLength(data.length()) {}

        [[nodiscard]] size_t length() const {
            return mLength;
        }

        [[nodiscard]] const char * data() const {
            return mData;
        }

        char operator[](size_t i) {
            return mData[i];
        }

        operator const char *() {
            return mData;
        }



        const_iterator begin() const {
            return mData;
        }

        const_iterator end() const {
            return mData + mLength;
        }

    private:
        const char * mData;
        size_t mLength;
    };

    bool operator==(const string_view & lhs, const string_view & rhs) {
        return lhs.length() == rhs.length() && strncmp(lhs.data(), rhs.data(), rhs.length()) == 0;
    }

    bool operator!=(const string_view & lhs, const string_view & rhs) {
        return !(lhs == rhs);
    }
}