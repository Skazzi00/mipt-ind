#pragma once

#include <cmath>
#include <cstdio>
#include <SFML/Graphics.hpp>

#include "stack.hpp"
#include "instructions.hpp"
#include "cpputils.hpp"
#include "bytecode.hpp"

namespace mipt::vasm {

    class Processor {
        using WordType = double;
        CheckedStack<WordType> stack;
        CheckedStack<const unsigned char *> callStack;
        WordType reg[32];
        Vector<WordType> mem;
        const size_t gMem;
        const unsigned char *rpc;
        const unsigned char *code;
    public:

        explicit Processor(const unsigned char *code, size_t memSize)
                : stack(), callStack(), reg(), mem(memSize), gMem(memSize / 2), rpc(code), code(code) {}

        void start() {

            while (true) {
                auto cmd = static_cast<Instruction>(*rpc);
                switch (cmd) {
                    case Instruction::sin:
                        stackOp<1>([](WordType first) -> WordType {
                            return sin(first);
                        });
                        break;
                    case Instruction::cos:
                        stackOp<1>([](WordType first) -> WordType {
                            return cos(first);
                        });
                        break;
                    case Instruction::subr:
                        stackOp<2>([](WordType first, WordType second) -> WordType {
                            return second - first;
                        });
                        break;
                    case Instruction::sub:
                        stackOp<2>([](WordType first, WordType second) -> WordType {
                            return first - second;
                        });
                        break;
                    case Instruction::add:
                        stackOp<2>([](WordType first, WordType second) -> WordType {
                            return first + second;
                        });
                        break;
                    case Instruction::mul:
                        stackOp<2>([](WordType first, WordType second) -> WordType {
                            return first * second;
                        });
                        break;
                    case Instruction::div:
                        stackOp<2>([](WordType first, WordType second) -> WordType {
                            return first / second;
                        });
                        break;
                    case Instruction::divr:
                        stackOp<2>([](WordType first, WordType second) -> WordType {
                            return second / first;
                        });
                        break;
                    case Instruction::inv:
                        stackOp<1>([](WordType operand) -> WordType {
                            return -operand;
                        });
                        break;
                    case Instruction::sqrt:
                        stackOp<1>([](WordType operand) -> WordType {
                            return sqrt(operand);
                        });
                        break;
                    case Instruction::round:
                        stackOp<1>([](WordType operand) -> WordType {
                            return round(operand);
                        });
                        break;
                    case Instruction::push: {
                        rpc++;
                        unsigned char mask = *rpc;
                        rpc++;
                        WordType regValue = 0;
                        if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) {
                            regValue = reg[*rpc];
                            rpc++;
                        }
                        double num = 0;
                        if (mask & static_cast<unsigned char>(ComplexMask::NUMBER)) {
                            num = *reinterpret_cast<const WordType *>(rpc);
                            rpc += sizeof(WordType);
                        }
                        double val = 0;
                        if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) {
                            val += regValue;
                        }
                        if (mask & static_cast<unsigned char>(ComplexMask::NUMBER)) {
                            val += num;
                        }
                        if (mask & static_cast<unsigned char>(ComplexMask::MEMORY)) {
                            val = mem[static_cast<size_t>(val)];
                        }
                        stack.push(val);
                    }
                        break;
                    case Instruction::pop:
                        stack.pop();
                        rpc++;
                        break;
                    case Instruction::top: {
                        rpc++;
                        unsigned char mask = *rpc;
                        rpc++;
                        WordType regValue = 0;
                        if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) {
                            regValue = mask & static_cast<unsigned char>(ComplexMask::MEMORY) ? reg[*rpc] : *rpc;
                            rpc++;
                        }
                        double num = 0;
                        if (mask & static_cast<unsigned char>(ComplexMask::NUMBER)) {
                            num = *reinterpret_cast<const WordType *>(rpc);
                            rpc += sizeof(WordType);
                        }
                        double val = 0;
                        if (mask & static_cast<unsigned char>(ComplexMask::REGISTER)) {
                            val += regValue;
                        }
                        if (mask & static_cast<unsigned char>(ComplexMask::NUMBER)) {
                            val += num;
                        }
                        if (mask & static_cast<unsigned char>(ComplexMask::MEMORY)) {
                            mem[static_cast<size_t>(val)] = stack.top();
                        } else {
                            reg[static_cast<size_t>(val)] = stack.top();
                        }
                    }
                        break;
                    case Instruction::in:
                        rpc++;
                        scanf("%lf", &reg[(*rpc)]);
                        rpc++;
                        break;
                    case Instruction::out:
                        printf("%lg\n", stack.top());
                        rpc++;
                        break;
                    case Instruction::dump:
                        dump(&stack, validate(&stack));
                        rpc++;
                        break;
                    case Instruction::hlt:
                        return;
                    case Instruction::pushd:
                        ++rpc;
                        stack.push(*reinterpret_cast<const WordType *>(rpc));
                        rpc += sizeof(WordType);
                        break;
                    case Instruction::jmp:
                        ++rpc;
                        rpc = code + *reinterpret_cast<const size_t *>(rpc);
                        break;
                    case Instruction::jz:
                        ++rpc;
                        if (stack.top() == 0)
                            rpc = code + *reinterpret_cast<const size_t *>(rpc);
                        else
                            rpc += sizeof(size_t);
                        break;
                    case Instruction::jg: {
                        ++rpc;
                        double first = stack.top();
                        stack.pop();
                        double second = stack.top();
                        stack.push(first);
                        if (first > second)
                            rpc = code + *reinterpret_cast<const size_t *>(rpc);
                        else
                            rpc += sizeof(size_t);
                    }
                        break;
                    case Instruction::jle: {
                        ++rpc;
                        double first = stack.top();
                        stack.pop();
                        double second = stack.top();
                        stack.push(first);
                        if (first <= second)
                            rpc = code + *reinterpret_cast<const size_t *>(rpc);
                        else
                            rpc += sizeof(size_t);
                    }
                        break;

                    case Instruction::call:
                        ++rpc;
                        callStack.push(rpc + sizeof(size_t));
                        rpc = code + *reinterpret_cast<const size_t *>(rpc);
                        break;
                    case Instruction::ret:
                        rpc = callStack.top();
                        callStack.pop();
                        break;
                    case Instruction::draw: {
                        const unsigned  windowSize = static_cast<const unsigned >(sqrt(gMem));
                        sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "My window");
                        sf::RectangleShape rectangle(sf::Vector2f(5, 5));
                        for (unsigned y = 0; y < windowSize; y++) {
                            for (unsigned x = 0; x < windowSize; ++x) {
                                rectangle.setFillColor(sf::Color(
                                        static_cast<unsigned int>(round(mem[gMem + y * windowSize + x]))));
                                rectangle.setPosition(static_cast<float>(x), static_cast<float>(y));
                                window.draw(rectangle);
                            }
                        }
                        window.display();
                        while (window.isOpen()) {
                            sf::Event event{};
                            while (window.waitEvent(event)) {
                                // "close requested" event: we close the window
                                if (event.type == sf::Event::Closed)
                                    window.close();
                            }
                        }
                        rpc++;
                    }
                        break;

                }
            }
        }

    private:


        template<typename Func, std::size_t... I>
        auto applyOperation(Func func, const WordType *args, index_sequence<I...>) {
            return func(args[I]...);
        }

        template<std::size_t argCnt, typename Func>
        void stackOp(Func func) {
            WordType args[argCnt];
            for (std::size_t i = 0; i < argCnt; ++i) {
                args[i] = stack.top();
                stack.pop();
            }
            stack.push(applyOperation(func, args, make_index_sequence<argCnt>{}));
            rpc++;
        }

    };

}