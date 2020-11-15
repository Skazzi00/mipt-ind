#pragma once

#include <stdexcept>
#include <cstring>
#include <string>

#include "errors.hpp"

namespace mipt::vasm {
    enum class Register : unsigned char {
        #define REG(name, val) name = (val),

        #include "registers.inl"
    };

    Register strToReg(const char *str) {
        #define REG(name, _) if (strcmp(str, #name) == 0) return Register::name;

        #include "registers.inl"

        throw parse_error(std::string("Not a register :").append(str));
    }

    Register strToRegN(const char *str, size_t length) {
    #define REG(name, _) if (strncmp(str, #name, length) == 0) return Register::name;

    #include "registers.inl"

        throw parse_error(std::string("Not a register :").append(str, length));
    }

    const char *regToStr(Register reg) {
        switch (reg) {
            #define REG(name, _) case Register::name: return #name;

            #include "registers.inl"
        }

        throw parse_error(std::string("Not a register:")
                                  .append(std::to_string(static_cast<unsigned>(reg))));
    }
}