#define MARKER_PROTECTION

#include "vector.hpp"

#undef MARKER_PROTECTION

namespace mipt {
    template<typename T, typename Container = Vector<T>>
    struct Stack {
        using container_type = Container;
        using value_type = typename Container::value_type;
        using size_type = typename Container::size_type;
        using reference = typename Container::reference;
        using const_reference = typename Container::const_reference;

        Stack() : data() {}


        reference top() {
            return data.back();
        }

        const_reference top() const {
            return data.back();
        }

        [[nodiscard]] size_type capacity() const {
            return data.capacity();
        }

        [[nodiscard]] size_type size() const {
            return data.size();
        }

        void push(const_reference val) {
            data.push_back(val);
        }

        void pop() {
            data.pop_back();
        }

    private:
        Container data;
    };
}