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
        static const size_t MIN_SIZE = 16;

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

            Node(Node &&other) noexcept: FakeNode(other.next, other.prev), value(other.value) {
                this->next->prev = this;
                this->prev->next = this;
            }

            void assign(const T & val) {
                value = val;
            }

            void assign(T&& val) {
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

        List() : mData(MIN_SIZE), mSize(0), free(), head(), optimized(false) {
            FakeNode *prev = &free;
            for (Node &item : mData) {
                linkNodes(prev, &item);
                prev = &item;
            }
            linkNodes(prev, &free);
        }

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

        List &operator=(const List &other) noexcept { // O(1)
            List tmp(other);
            swap(tmp);
            return *this;
        }

        List(List &&other) noexcept: List() { // O(1)
            swap(other);
        }

        List &operator=(List &&other) noexcept { // O(1)
            swap(other);
            return *this;
        }

        void swap(List &other) {
            std::swap(other.mData, mData);
            std::swap(other.mSize, mSize);
            std::swap(other.optimized, optimized);
            std::swap(other.head, head);
            std::swap(other.free, free);
        }


        size_t capacity() const {
            return mData.size();
        }

        void pop_back() {
            decSize();
            Node *popped = static_cast<Node *>(head.prev);
            popped->cut();
            addToFree(popped);
        }

        void pop_front() {
            decSize();
            Node *popped = static_cast<Node *>(head.next);
            popped->cut();
            addToFree(popped);
        }

        template<typename... Args>
        void emplace_back(Args &&... args) {
            Node *freeNode = getFreeNode();
            freeNode->assign( T(std::forward<Args>(args)...));
            FakeNode *last = head.prev;
            linkNodes(last, freeNode);
            linkNodes(freeNode, &head);
            incSize();
            if (free.next == &free) {
                mData.emplace_back();
                addToFree(&mData.back());
            }
        }

        template<typename... Args>
        void emplace_front(Args &&... args) {
            Node *freeNode = getFreeNode();
            freeNode->assign(T(std::forward<Args>(args)...));
            Node *first = head.next;
            linkNodes(freeNode, first);
            linkNodes(head, freeNode);
            incSize();
            if (free.next == &free) {
                mData.emplace_back();
                addToFree(&mData.back());
            }
        }

        void push_back(const_reference val) {
            emplace_back(val);
        }

        void push_back(value_type &&val) {
            emplace_back(std::move(val));
        }

        template<typename... Args>
        void emplace(const_iterator it, Args &&... args) {
            Node *curNode = const_cast<Node *>(it.node());
            Node *newNode = getFreeNode();
            newNode->assign(T(std::forward<Args>(args)...));
            linkNodes(curNode->prev, newNode);
            linkNodes(newNode, curNode);
            incSize();
            if (free.next == &free) {
                mData.emplace_back();
                addToFree(&mData.back());
            }
        }

        void erase(const_iterator pos) {
            decSize();
            Node *erased = const_cast<Node *>(pos.node());
            erased->cut();
            addToFree(erased);
        }

        iterator get(size_t index) {
            if (optimized) {
                return iterator(&mData[index]);
            } else {

                iterator result = begin();
                std::advance(result, static_cast<long>(index));
                return result;
            }
        }

        const_iterator get(size_t index) const {
            if (optimized) {
                return const_iterator(&mData[index]);
            } else {
                const_iterator result = begin();
                std::advance(result, static_cast<long>(index));
                return result;
            }
        }

        T &front() {
            return *begin();
        }

        const T &front() const {
            return *begin();
        }

        T &back() {
            auto res = end();
            res--;
            return *res;
        }

        const T &back() const {
            auto res = end();
            res--;
            return *res;
        }

        size_t size() const {
            return mSize;
        }

        bool empty() const {
            return size() == 0;
        }

        iterator begin() {
            return iterator(static_cast<Node *>(head.next));
        }

        const_iterator begin() const {
            return const_iterator(static_cast<const Node *>(head.next));
        }

        iterator end() {
            return iterator(static_cast<Node *>(&head));
        }

        const_iterator end() const {
            return const_iterator(static_cast<const Node *>(&head));
        }

        void optimize() {
            if (optimized) return;
            List<value_type> tmp(*this);
            swap(tmp);
        }

    private:
        void incSize() {
            optimized = false;
            mSize++;
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