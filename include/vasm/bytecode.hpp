#pragma once

#include <cstddef>

#include "string_view.hpp"

namespace mipt::vasm {

    constexpr double VERSION = 1.0;

    struct ByteCodeMetadata {
        size_t length;
        double version = VERSION;
    };

    struct Label {
        string_view name;
        size_t address;

        Label(string_view name, size_t addr) : name(name), address(addr) {}
    };

    enum class ComplexMask : unsigned char {
        NUMBER = 1,
        REGISTER = 2,
        MEMORY = 4
    };
}