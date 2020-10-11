#pragma once

#define MARKER_PROTECTION

#include "vector.hpp"
#include "data_structures.hpp"

#undef MARKER_PROTECTION

namespace mipt {
    template<typename T, typename Container = Vector<T>>
    struct Stack {
        using container_type = Container;
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;

        Stack() : mData() {}


        reference top() {
            return mData.back();
        }

        const_reference top() const {
            return mData.back();
        }

        [[nodiscard]] size_type capacity() const {
            return mData.capacity();
        }

        [[nodiscard]] size_type size() const {
            return mData.size();
        }

        template<typename... Args>
        void emplace(Args &&... args) {
            mData.emplace_back(std::forward<Args>(args)...);
        }

        void push(const_reference val) {
            mData.push_back(val);
        }

        void push(value_type &&val) {
            mData.push_back(std::move(val));
        }

        void pop() {
            mData.pop_back();
        }

        void swap(Stack &other) {
            std::swap(mData, other.mData);
        }

        friend Status validate(Stack const *stack) {
            return validate(&stack->mData);
        }

        template<typename PrintFunc>
        friend void dump(Stack const *st, PrintFunc printFunc,
                         Status status = Status::OK,
                         FILE *logfile = stderr,
                         int level = 0) {
            fprintf(logfile, "Stack<%s> (%s) [0x%zX] {\n", typeid(value_type).name(), statusToStr(status),
                    reinterpret_cast<size_t>(st));
            if (!st) {
                fprintf(
                        logfile,
                        "\tData unavailable\n"
                        "}\n"
                );
                return;
            }
            fprintf(logfile, "\tmData {\n");
            dump(&st->mData, printFunc, status, logfile, level + 5);
            fprintf(logfile, "\t}\n");
            fprintf(logfile, "}\n");
        }

    private:
        Container mData;
    };
}