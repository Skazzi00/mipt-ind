#pragma once

#include <cstring>
#include <string>

#include "vasm/errors.hpp"

namespace mipt::vasm {
    const std::size_t MAX_CMD_LEN = 32;
    const char        COMMENT     = ';';

    enum class Instruction {
        #define INST(name, code, _, __) name = (code) ,

        #include "instructions.inl"
    };


    const char *toString(Instruction i) {
        switch (i) {
            #define INST(name, _, __, ___) case Instruction::name: return #name;

            #include "instructions.inl"

            default:
                throw parse_error(std::string("Unknown instruction")
                                          .append(std::to_string(static_cast<unsigned>(i))));
        }
    }


}