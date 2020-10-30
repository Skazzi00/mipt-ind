#pragma once

#include <cstring>
#include <cstdlib>

#include "vasm/instructions.hpp"
#include "vasm/registers.hpp"
#include "vasm/errors.hpp"
#include "stack.hpp"
#include "string_view.hpp"
#include "vasm/bytecode.hpp"

namespace mipt::vasm {



    struct ParsedData {
        Vector<Label> labels;
        Vector<string_view> tokens;

        explicit ParsedData(char *data) : labels(), tokens() {
            string_view token = strtok(data, " \n");
            size_t addr = 0;
            while (token.data()) {
                if (token[0] == COMMENT) {
                    token = strtok(nullptr, "\n");
                    token = strtok(nullptr, " \n");
                    continue;
                }
                if (token[token.length() - 1] == ':') {
                    labels.emplace_back(string_view(token, token.length() - 1), addr);
                    token = strtok(nullptr, " \n");
                    continue;
                }
                bool isParsed = false;

                #define INST_DOUBLE addr += sizeof(double); \
                tokens.push_back(token = strtok(nullptr, " \n"));

                #define INST_REGISTER addr++; \
                tokens.push_back(token = strtok(nullptr, " \n"));

                #define INST_LABEL addr += sizeof(size_t); \
                tokens.push_back(token = strtok(nullptr, " \n"));

                #define INST_COMPLEX parseComplex();

                const auto parseComplex = [&token, &addr, this]() {
                    token = strtok(nullptr, " \n");
                    bool reg = false;
                    bool num = false;
                    for (size_t i = 0; i < token.length(); ++i) {
                        if (isalpha(token[i])) {
                            reg = true;
                        }

                        if (isdigit(token[i])) {
                            num = true;
                        }
                        if (reg && num) break;
                    }
                    addr++; //mask byte
                    if (reg) addr++;
                    if (num) addr += sizeof(double);
                    tokens.push_back(token);
                };

                #define INST(name, code, _, execCode)  \
                if (strcmp(token, #name) == 0) {       \
                    isParsed = true;                   \
                    addr++;                            \
                    tokens.push_back(token);           \
                    execCode                           \
                }
                #include "instructions.inl"

                if (!isParsed) {
                    throw parse_error("Unknown instruction");
                }
                token = strtok(nullptr, " \n");
            }
        }
    };



    CheckedStack<unsigned char> compile(char *data) {
        CheckedStack<unsigned char> byteCode;
        auto parsed = ParsedData(data);
        auto curToken = parsed.tokens.begin();

        const auto parseRegister = [&curToken, &byteCode]() {
            curToken++;
            auto reg = static_cast<unsigned char>(strToReg(*curToken));
            byteCode.push(reg);
        };

        const auto parseDouble = [&curToken, &byteCode]() {
            curToken++;
            double num = strtod(*curToken, nullptr);
            auto ptr = reinterpret_cast<unsigned char *>(&num);
            for (size_t i = 0; i < sizeof(num); ++i) {
                byteCode.push(*ptr);
                ptr++;
            }
        };

        const auto parseLabel = [&curToken, &byteCode, &parsed]() {
            curToken++;
            for (const auto & label : parsed.labels) {
                if (label.name == *curToken) {
                    auto ptr = reinterpret_cast<const unsigned char *>(&label.address);
                    for (size_t i = 0; i < sizeof(label.address); ++i) {
                        byteCode.push(*ptr);
                        ptr++;
                    }
                    return;
                }
            }
        };

        const auto parseComplex = [&curToken, &byteCode]() {
            curToken++;
            string_view &tok = *curToken;
            unsigned char mask = 0;
            Register reg;
            double number = 0;
            size_t pos = 0;
            if (tok[0] == '[' && tok[tok.length() - 1] == ']') {
                mask |= static_cast<unsigned char>(ComplexMask::MEMORY);
                pos++;
            }
            size_t regLength = 0;
            while (pos + regLength < tok.length() && isalpha(tok[pos + regLength])) {
                regLength++;
            }
            if (regLength != 0) {
                mask |= static_cast<unsigned char>(ComplexMask::REGISTER);
                reg = strToRegN(tok + pos, regLength);
            }

            pos += regLength;

            if (pos < tok.length() && (tok[pos] == '+' || tok[pos] == '-' || isdigit(tok[pos]))) {
                mask |= static_cast<unsigned char>(ComplexMask::NUMBER);
                number = strtod(tok + pos, nullptr);
            }

            byteCode.push(mask);

            if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) {
                byteCode.push(static_cast<const unsigned char>(reg));
            }

            if (mask & static_cast<unsigned char>(ComplexMask::NUMBER)) {
                auto ptr = reinterpret_cast<unsigned char *>(&number);
                for (size_t i = 0; i < sizeof(number); ++i) {
                    byteCode.push(*ptr);
                    ptr++;
                }
            }
        };

        while (curToken != parsed.tokens.end()) {

            #define INST_DOUBLE parseDouble();
            #define INST_REGISTER parseRegister();
            #define INST_LABEL parseLabel();
            #define INST_COMPLEX parseComplex();

            #define INST(name, code, _, parseCode) \
            if (strcmp(*curToken, #name) == 0) {   \
                byteCode.push(code);               \
                parseCode                          \
            }

            #include "instructions.inl"


            curToken++;
        }
        byteCode.shrink_to_fit();
        return byteCode;
    }
}