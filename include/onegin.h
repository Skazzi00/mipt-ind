#pragma once

#include <random>
#include <stack>
#include <iterator>

namespace onegin {

    template<class RandomIt, class Compare>
    RandomIt partition(RandomIt first, RandomIt last, const Compare cmp) {
        static std::mt19937 rng(std::random_device{}());
        const auto val = first[rng() % (std::distance(first, last))];
        --last;
        while (first <= last) {
            while (cmp(*first, val)) ++first;
            while (!cmp(*last, val)) --last;
            if (first >= last) break;
            std::iter_swap(first++, last--);
        }
        return last;
    }

    template<class RandomIt, class Compare>
    void sort(RandomIt first, RandomIt last, const Compare cmp) {
        std::stack<std::pair<RandomIt, RandomIt>> ranges;
        ranges.emplace(first, last);
        while (!ranges.empty()) {
            auto[l, r] = ranges.top();
            ranges.pop();
            if (r - l <= 1) continue;
            auto pos = partition(l, r, cmp);
            if (std::distance(l, pos) > std::distance(pos, r)) {
                ranges.emplace(l, pos);
                ranges.emplace(pos, r);
            } else {
                ranges.emplace(pos, r);
                ranges.emplace(l, pos);
            }
        }
    }

    template<class RandomIt>
    void sort(RandomIt first, RandomIt last) {
        sort(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
    }
}