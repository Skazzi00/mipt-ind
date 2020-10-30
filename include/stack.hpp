#pragma once

#include <cstdio>

#define MARKER_PROTECTION
#define HASH_PROTECTION

#include "vector.hpp"
#include "data_structures.hpp"


#ifdef HASH_PROTECTION
#define REHASH() do{ mHash = std::hash<Container>{}(mData); } while (0)
#else
#define REHASH()
#endif

#define ASSERT_OK()                                         \
    do {                                                    \
        Status status = validate(this);                     \
        if (status != Status::OK) {                         \
            dump(this, status);                   \
            throw std::runtime_error(statusToStr(status));  \
        }                                                   \
    } while(0)                                              \

#define WRAP_ASSERT(statement) \
         do {                  \
            ASSERT_OK();       \
            statement;         \
            REHASH();          \
            ASSERT_OK();       \
         }while(0)


namespace mipt {
    template<typename T, typename Container = Vector<T>>
    struct CheckedStack {
        using container_type = Container;
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;


        explicit CheckedStack() : mData() {
            REHASH();
        }


        value_type top() const {
            ASSERT_OK();
            return mData.back();
        }

        [[nodiscard]] size_type capacity() const {
            ASSERT_OK();
            return mData.capacity();
        }

        [[nodiscard]] size_type size() const {
            ASSERT_OK();
            return mData.size();
        }

        template<typename... Args>
        void emplace(Args &&... args) {
            WRAP_ASSERT(mData.emplace_back(std::forward<Args>(args)...));
        }

        void push(const_reference val) {
            WRAP_ASSERT(mData.push_back(val));
        }

        void push(value_type &&val) {
            WRAP_ASSERT(mData.push_back(std::move(val)));
        }

        void pop() {
            WRAP_ASSERT(mData.pop_back());
        }

        void swap(CheckedStack &other) {
            WRAP_ASSERT(std::swap(mData, other.mData));
        }

        void shrink_to_fit() {
            WRAP_ASSERT(mData.shrink_to_fit());
        }

        T *data() {
            ASSERT_OK();
            return mData.data();
        }

        friend Status validate(CheckedStack const *stack) {
            if (!stack) {
                return Status::NULL_PTR;
            }
#ifdef HASH_PROTECTION
            if (stack->mHash != std::hash<Container>{}(stack->mData)) {
                return Status::CORRUPTED_HASH;
            }
#endif
            return validate(&stack->mData);
        }

        friend void dump(CheckedStack const *st,
                         Status status = Status::OK,
                         FILE *logfile = stderr,
                         int level = 0) {
            fprintf(logfile, "CheckedStack<%s> (%s) [0x%zX] {\n", typeid(value_type).name(), statusToStr(status),
                    reinterpret_cast<size_t>(st));
            if (!st) {
                fprintf(
                        logfile,
                        "\tData unavailable\n"
                        "}\n"
                );
                return;
            }
#ifdef HASH_PROTECTION
            fprintf(logfile, "\tmHash = %zX ", st->mHash);
            if (status == Status::CORRUPTED_HASH) {
                fprintf(logfile, "(ACTUAL = %zX)", std::hash<Container>{}(st->mData));
            }
            fprintf(logfile, "\n");
#endif
            fprintf(logfile, "\tmData {\n");
            dump(&st->mData, status, logfile, level + 5); // + 5 for proper tabulation
            fprintf(logfile, "\t}\n");
            fprintf(logfile, "}\n");
        }

    private:
        Container mData;

#ifdef HASH_PROTECTION
        std::size_t mHash;
#endif
    };
}

#undef ASSERT_OK
#undef WRAP_ASSERT
