#pragma once

#include <cstdio>

#include "vasm/bytecode.hpp"
#include "vasm/registers.hpp"
#include "vector.hpp"
#include "cpputils.hpp"

namespace mipt::vasm {

    Vector<size_t> findLabelsAddress(ByteCodeMetadata metadata, const unsigned char *byteCode) {
        Vector<size_t> result;
        const unsigned char * ptr = byteCode;

        const auto addLabelAddress = [&result](const unsigned char *ptr) {
            if (find(result.begin(), result.end(), *reinterpret_cast<const size_t *>(ptr)) == result.end())
                result.emplace_back(*reinterpret_cast<const size_t *>(ptr));
        };
        while (ptr  - byteCode < static_cast<long>(metadata.length)) {
            switch (*ptr) {
                #define INST_LABEL addLabelAddress(ptr); ptr += sizeof(size_t);
                #define INST_DOUBLE ptr += sizeof(double );
                #define INST_REGISTER ptr++;
                #define INST_COMPLEX {                                                                  \
                    unsigned char mask = *ptr;                                                         \
                    ptr++;                                                                             \
                    if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) ptr++;                \
                    if (mask & static_cast<unsigned char>(ComplexMask::NUMBER))   ptr += sizeof(double); \
                }

                #define INST(name, code, _, execCode)      \
                    case code:                         \
                    ptr++;                             \
                    execCode                           \
                    break;

                #include "instructions.inl"
            }
        }
        std::sort(result.begin(), result.end());
        return result;
    }

    void decompile(ByteCodeMetadata metadata, unsigned char *byteCode, FILE *out) {
        unsigned char *ptr = byteCode;
        auto labelAddress = findLabelsAddress(metadata, byteCode);
        const auto printDouble = [&ptr, &out]() {
            fprintf(out, "%lg", *reinterpret_cast<double *>(ptr));
            ptr += sizeof(double);
        };
        const auto printRegister = [&ptr, &out]() {
            fprintf(out, "%s", regToStr(static_cast<Register>(*ptr)));
            ptr++;
        };
        const auto printLabel = [&ptr, &labelAddress, &out]() {
            auto label = find(labelAddress.begin(), labelAddress.end(), *reinterpret_cast<const size_t*>(ptr));
            fprintf(out, "label%ld", label - labelAddress.begin());
            ptr += sizeof(size_t);
        };

        const auto printComplex = [&ptr, &out] () {
            unsigned char mask = *ptr;
            ptr++;
            if (mask & static_cast<unsigned char>(ComplexMask::MEMORY)) {
                fprintf(out, "[");
            }
            if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) {
                fprintf(out, "%s", regToStr(static_cast<Register>(*ptr)));
                ptr++;
            }
            if (mask & static_cast<unsigned char>(ComplexMask::NUMBER)) {
                fprintf(out, "%+lg", *reinterpret_cast<double *>(ptr));
                ptr+=sizeof(double );
            }
            if (mask & static_cast<unsigned char>(ComplexMask::MEMORY)) {
                fprintf(out, "]");
            }
        };

        #define INST_DOUBLE printDouble();
        #define INST_REGISTER printRegister();
        #define INST_LABEL printLabel();
        #define INST_COMPLEX printComplex();

        while (ptr - byteCode < static_cast<long>(metadata.length)) {
            auto label = find(labelAddress.begin(), labelAddress.end(), static_cast<size_t>(ptr - byteCode));
            if (label != labelAddress.end()) {
                fprintf(out, "label%ld:\n", label - labelAddress.begin());
            }
            switch (*ptr) {
            #define INST(name, code, _, execCode) \
                case code:                        \
                fputs(#name " ", out);             \
                ptr++;                            \
                execCode                          \
                fputc('\n', out);                 \
                break;

            #include "instructions.inl"
            }
        }
    }
}