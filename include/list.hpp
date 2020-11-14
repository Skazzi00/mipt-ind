#pragma once

#include <utility>
#include <cstdio>
#include <algorithm>

#include "vector.hpp"


namespace mipt {


    template<typename T>
    struct List {
    private:
        template<typename ItT, typename NodeType>
        struct Iterator;
    public:

        struct FakeNode {
            FakeNode *next;
            FakeNode *prev;

            FakeNode() : next(this), prev(this) {}

            FakeNode(FakeNode *next, FakeNode *prev) : next(next), prev(prev) {}
        };

        struct Node : FakeNode {
            T value;

            Node() : FakeNode(), value() {}

            Node(T val) : FakeNode(), value(val) {}

            Node(const Node &) = delete;

            Node &operator=(const Node &) = delete;

            Node(Node &&other) noexcept: FakeNode(other.next, other.prev), value(other.value) {
                this->next->prev = this;
                this->prev->next = this;
            }

            void assign(const T &val) {
                value = val;
            }

            void assign(T &&val) {
                value = std::move(val);
            }

            Node &operator=(Node &&other) {
                swap(other);
                return *this;
            }

            void swap(Node &other) {
                using std::swap;
                swap(other.value, value);
                swap(other.next, this->next);
                swap(other.prev, this->prev);
            }

            void cut() {
                value = T();
                this->next->prev = this->prev;
                this->prev->next = this->next;
            }

            operator typename List<const T>::Node() const {
                return typename List<const T>::Node(*this);
            }
        };

        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = Iterator<T, List<T>::Node>;
        using const_iterator = Iterator<const T, const List<T>::Node>;

        List() noexcept: mData(), mSize(0), free(), head(), optimized(false) {}

        List(const List &other) : mData(), mSize(0), free(), head(), optimized(true) {
            FakeNode *prev = &head;
            for (const auto &item : other) {
                mData.push_back(item);
                linkNodes(prev, &mData.back());
                prev = &mData.back();
                mSize++;
            }
            linkNodes(prev, &head);
        }

        List &operator=(const List &other) noexcept {
            List tmp(other);
            swap(tmp);
            return *this;
        }

        List(List &&other) noexcept: List() {
            swap(other);
        }

        List &operator=(List &&other) noexcept {
            swap(other);
            return *this;
        }

        void swap(List &other) noexcept {
            using std::swap;
            swap(other.mData, mData);
            swap(other.mSize, mSize);
            swap(other.optimized, optimized);
            swap(other.head, head);
            swap(other.free, free);
        }

        void pop_back() noexcept {
            erase(--end());
        }

        void pop_front() noexcept {
            erase(begin());
        }

        iterator get(size_t index) noexcept {
            if (optimized) {
                return iterator(&mData[index]);
            } else {
                iterator result = begin();
                for (size_t i = 0; i < index; i++, result++) {};
                return result;
            }
        }

        const_iterator get(size_t index) const noexcept {
            if (optimized) {
                return const_iterator(&mData[index]);
            } else {
                const_iterator result = begin();
                for (size_t i = 0; i < index; i++, result++) {};
                return result;
            }
        }

        template<typename... Args>
        void emplace_back(Args &&... args) {
            emplace(end(), std::forward<Args>(args)...);
        }

        template<typename... Args>
        void emplace_front(Args &&... args) {
            emplace(begin(), std::forward<Args>(args)...);
        }

        void push_back(const_reference val) {
            emplace_back(val);
        }

        void push_back(value_type &&val) {
            emplace_back(std::move(val));
        }

        void push_front(const_reference val) {
            emplace_front(val);
        }

        void push_front(value_type &&val) {
            emplace_front(std::move(val));
        }

        template<typename... Args>
        void emplace(const_iterator it, Args &&... args) {
            Node *curNode = const_cast<Node *>(it.node());
            Node *newNode = getFreeNode();
            newNode->assign(T(std::forward<Args>(args)...));
            linkNodes(curNode->prev, newNode);
            linkNodes(newNode, curNode);
            incSize();
        }

        void insert(const_iterator it, const_reference val) {
            emplace(it, val);
        }

        void insert(const_iterator it, value_type &&val) {
            emplace(it, std::move(val));
        }

        void erase(const_iterator pos) noexcept {
            decSize();
            Node *erased = const_cast<Node *>(pos.node());
            erased->cut();
            addToFree(erased);
        }


        T &front() noexcept {
            return *begin();
        }

        const T &front() const noexcept {
            return *begin();
        }

        T &back() noexcept {
            auto res = end();
            res--;
            return *res;
        }

        const T &back() const noexcept {
            auto res = end();
            res--;
            return *res;
        }

        size_t size() const noexcept {
            return mSize;
        }

        bool empty() const noexcept {
            return size() == 0;
        }

        iterator begin() noexcept {
            return iterator(static_cast<Node *>(head.next));
        }

        const_iterator begin() const noexcept {
            return const_iterator(static_cast<const Node *>(head.next));
        }

        iterator end() noexcept {
            return iterator(static_cast<Node *>(&head));
        }

        const_iterator end() const noexcept {
            return const_iterator(static_cast<const Node *>(&head));
        }

        iterator beginFree() noexcept {
            return iterator(static_cast<Node *>(free.next));
        }

        const_iterator beginFree() const noexcept {
            return const_iterator(static_cast<const Node *>(free.next));
        }

        iterator endFree() noexcept {
            return iterator(static_cast<Node *>(&free));
        }

        const_iterator endFree() const noexcept {
            return const_iterator(static_cast<const Node *>(&free));
        }

        void optimize() {
            if (optimized) return;
            List<value_type> tmp(*this);
            swap(tmp);
        }

        friend void dump(const List &list, FILE *out) {
            fprintf(out, "digraph list {\n"
                         "graph [rankdir = \"LR\"];\n"
                         "rank = same;\n"
                         "node[shape = record];\n");

            auto dumpComponent = [&list, &out](const char *prefix,
                                               const char *nodeColor,
                                               const char *endColor,
                                               List<T>::const_iterator begin,
                                               List<T>::const_iterator end) {
                for (auto it = begin; it != end; ++it) {
                    long index = it.node() - list.mData.begin();
                    fprintf(out, "\"%s%ld\" [label = \"<index> %ld | <prev> prev | <next> next | <val> value = ",
                            prefix, index,
                            index);
                    printToFile(out, *it);
                    fprintf(out, "\", color=%s];\n", nodeColor);
                }
                fprintf(out, "\"%s%d\" [label = \"<index> END | <prev> prev | <next> next\", color=%s];\n", prefix, -1,
                        endColor);

                for (auto it = begin; it != end; ++it) {
                    long nodeIndex = it.node() - list.mData.begin();
                    long prevIndex =
                            it.node()->prev == end.node()
                            ? -1
                            : static_cast<Node *>(it.node()->prev) - list.mData.begin();
                    long nextIndex =
                            it.node()->next == end.node()
                            ? -1
                            : static_cast<Node *>(it.node()->next) - list.mData.begin();

                    fprintf(out, "\"%s%ld\":prev->\"%s%ld\":index;\n", prefix, nodeIndex, prefix, prevIndex);
                    fprintf(out, "\"%s%ld\":next->\"%s%ld\":index;\n", prefix, nodeIndex, prefix, nextIndex);
                }

                long endPrevIndex =
                        end.node()->prev == &list.head ? -1 : static_cast<Node *>(end.node()->prev) -
                                                              list.mData.begin();
                long endNextIndex =
                        end.node()->next == &list.head ? -1 : static_cast<Node *>(end.node()->next) -
                                                              list.mData.begin();

                fprintf(out, "\"%s-1\":prev->\"%s%ld\":index;\n", prefix, prefix, endPrevIndex);
                fprintf(out, "\"%s-1\":next->\"%s%ld\":index;\n", prefix, prefix, endNextIndex);

            };

            dumpComponent("node", "blue", "blueviolet", list.begin(), list.end());
            dumpComponent("freeNode", "green", "greenyellow", list.beginFree(), list.endFree());

            fprintf(out, "}\n");
        }

        friend Status validate(const List &list) {
            Status vecCode = validate(&list.mData);
            if (vecCode != Status::OK) {
                return vecCode;
            }

            auto it = list.begin();
            for (int i = 0; i < list.size() + 1; ++i, ++it) {}
            if (it != list.begin()) return Status::INVALID;

            for (int i = 0; i < list.size() + 1; ++i, --it) {}
            if (it != list.begin()) return Status::INVALID;

            auto freeIt = list.beginFree();
            long freeSize = static_cast<long>(list.mData.size()) - static_cast<long>(list.size());
            for (int i = 0; i < freeSize + 1; ++i, ++freeIt) {}
            if (freeIt != list.beginFree()) return Status::INVALID;

            for (int i = 0; i < freeSize + 1; ++i, --freeIt) {}
            if (freeIt != list.beginFree()) return Status::INVALID;

            return Status::OK;
        }

    private:
        void incSize() {
            optimized = false;
            mSize++;
            if (free.next == &free) {
                mData.emplace_back();
                addToFree(&mData.back());
            }
        }

        void decSize() {
            optimized = false;
            mSize--;
        }

        template<typename ItT, typename NodeType>
        struct Iterator {
            NodeType *cur;
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = ItT;
            using difference_type = std::ptrdiff_t;
            using pointer = ItT *;
            using reference = ItT &;

            Iterator(NodeType *cur) : cur(cur) {}

            ItT &operator*() const {
                return cur->value;
            }

            NodeType *node() const {
                return cur;
            }

            ItT *operator->() const {
                return &cur->value;
            }

            Iterator &operator++() {
                cur = static_cast<NodeType *> (cur->next);
                return *this;
            }

            Iterator operator++(int) {
                Iterator tmp = *this;
                ++(*this);
                return tmp;
            }

            Iterator &operator--() {
                cur = static_cast<NodeType *>(cur->prev);
                return *this;
            }

            Iterator operator--(int) {
                Iterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const Iterator &rhs) const {
                return cur == rhs.cur;
            }

            bool operator!=(const Iterator &rhs) const {
                return !(rhs == *this);
            }

            operator const_iterator() const {
                return const_iterator(cur);
            }
        };

        void addToFree(FakeNode *v) {
            linkNodes(v, free.next);
            linkNodes(&free, v);
        }

        Node *getFreeNode() {
            if (free.next == &free) {
                mData.emplace_back();
                addToFree(&mData.back());
            }
            FakeNode *res = free.next;
            linkNodes(&free, res->next);
            return static_cast<Node *>(res);
        }

        void linkNodes(FakeNode *left, FakeNode *right) {
            left->next = right;
            right->prev = left;
        }

        Vector<Node> mData;
        size_t mSize;
        FakeNode free;
        FakeNode head;
        bool optimized;
    };
}