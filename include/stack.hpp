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

        template<typename... Args>
        void emplace(Args &&... args) {
            data.emplace_back(std::forward<Args>(args)...);
        }

        void push(const_reference val) {
            data.push_back(val);
        }

        void push(value_type &&val) {
            data.push_back(std::move(val));
        }

        void pop() {
            data.pop_back();
        }

        void swap(Stack & other) {
            std::swap(data, other.data);
        }
    private:
        Container data;
    };
}