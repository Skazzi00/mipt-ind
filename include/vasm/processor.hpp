#pragma once

#include <cmath>
#include <cstdio>
#include <SFML/Graphics.hpp>

#include "stack.hpp"
#include "instructions.hpp"
#include "cpputils.hpp"
#include "bytecode.hpp"

namespace mipt::vasm {

    const float PIXEL_SIZE = 5.;

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
                rpc++;
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
                        ComplexValue complex = scanComplex();
                        WordType value = complex.num;
                        if (complex.isRegister) {
                            value += reg[static_cast<size_t>(complex.reg)];
                        }
                        if (complex.isMemory) {
                            value = mem[static_cast<size_t>(value)];
                        }
                        stack.push(value);
                    }
                        break;

                    case Instruction::top: {
                        ComplexValue complex = scanComplex();
                        if (complex.isMemory) {
                            WordType regVal = complex.isRegister ? reg[static_cast<size_t>(complex.reg)] : 0;
                            auto index = static_cast<size_t>(regVal + complex.num);
                            mem[index] = stack.top();
                        } else if (complex.isRegister) {
                            reg[static_cast<size_t>(complex.reg)] = stack.top();
                        }
                    }
                        break;

                    case Instruction::pop:
                        stack.pop();
                        break;

                    case Instruction::in:
                        scanf("%lf", &reg[(*rpc)]);
                        rpc++;
                        break;

                    case Instruction::out:
                        printf("%lg\n", stack.top());
                        break;

                    case Instruction::dump:
                        dump(&stack, validate(&stack));
                        break;

                    case Instruction::hlt:
                        return;

                    case Instruction::pushd:
                        stack.push(scanWord());
                        break;

                    case Instruction::jmp:
                        jump();
                        break;

                    case Instruction::meow:
                        stackOp<1>([](WordType top)  -> WordType {
                            for(int i = 0; i < top; i++) {
                                system("say meow");
                            }
                            return top;
                        });
                        break;
                    case Instruction::jz:
                        jumpIfStack<1>([](WordType top) -> bool {
                            return top == 0.;
                        });
                        break;

                    case Instruction::jg:
                        jumpIfStack<2>([](WordType first, WordType second) -> bool {
                            return first > second;
                        });
                        break;

                    case Instruction::jle:
                        jumpIfStack<2>([](WordType first, WordType second) -> bool {
                            return first <= second;
                        });
                        break;

                    case Instruction::call:
                        callStack.push(rpc + sizeof(size_t));
                        jump();
                        break;

                    case Instruction::ret:
                        rpc = callStack.top();
                        callStack.pop();
                        break;

                    case Instruction::draw: {
                        const unsigned windowSize = static_cast<const unsigned>(sqrt(gMem));
                        sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "My window");
                        sf::RectangleShape rectangle(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));

                        for (unsigned y = 0; y < windowSize; ++y) {
                            for (unsigned x = 0; x < windowSize; ++x) {
                                rectangle.setFillColor(sf::Color(
                                        static_cast<unsigned int>(round(mem[gMem + y * windowSize + x]))
                                ));
                                rectangle.setPosition(static_cast<float>(x), static_cast<float>(y));
                                window.draw(rectangle);
                            }
                        }

                        window.display();
                        while (window.isOpen()) {
                            sf::Event event{};
                            while (window.waitEvent(event)) {
                                if (event.type == sf::Event::Closed)
                                    window.close();
                            }
                        }
                    }
                        break;

                }
            }
        }

    private:

        struct ComplexValue {
            bool isMemory;
            bool isRegister;
            bool isNumber;
            WordType reg;
            WordType num;
        };

        ComplexValue scanComplex() {
            unsigned char mask = *rpc;
            ComplexValue result{
                    static_cast<bool>(mask & static_cast<unsigned char>(ComplexMask::MEMORY)),
                    static_cast<bool>(mask & static_cast<unsigned char>(ComplexMask::REGISTER)),
                    static_cast<bool>(mask & static_cast<unsigned char>(ComplexMask::NUMBER)),
                    0,
                    0
            };
            rpc++;

            if (result.isRegister) {
                result.reg = *rpc;
                rpc++;
            }

            if (result.isNumber) {
                result.num = *reinterpret_cast<const WordType *>(rpc);
                rpc += sizeof(WordType);
            }
            return result;
        }

        WordType scanWord() {
            WordType result = *reinterpret_cast<const WordType *>(rpc);
            rpc += sizeof(WordType);
            return result;
        }

        void jump() {
            rpc = code + *reinterpret_cast<const size_t *>(rpc);
        }

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
        }

        template<std::size_t argCnt, typename Pred>
        void jumpIfStack(Pred predicate) {
            WordType args[argCnt];
            for (std::size_t i = 0; i < argCnt; ++i) {
                args[i] = stack.top();
                stack.pop();
            }
            if (applyOperation(predicate, args, make_index_sequence<argCnt>{})) {
                jump();
            } else {
                rpc += sizeof(size_t);
            }
            for (std::size_t i = argCnt; i >= 1; i--) {
                stack.push(args[i - 1]);
            }
        }

    };
}