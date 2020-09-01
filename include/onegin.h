#pragma once

#include <random>
#include <stack>
#include <iterator>

namespace onegin {

    template<class RandomIt, class Compare>
    RandomIt partition(RandomIt first, RandomIt last, const Compare cmp) {
        assert(first <= last);
        static std::mt19937 rng(std::random_device{}());
        const auto val = first[rng() % static_cast<std::size_t>(std::distance(first, last + 1))];
        while (first <= last) {
            while (cmp(*first, val)) ++first;
            while (cmp(val, *last)) --last;
            if (first >= last) break;
            std::iter_swap(first++, last--);
        }
        return last;
    }

    template<class RandomIt, class Compare>
    void sort(RandomIt first, RandomIt last, const Compare cmp) {
        assert(first <= last);
        std::stack<std::pair<RandomIt, RandomIt>> ranges;
        ranges.emplace(first, last - 1);
        while (!ranges.empty()) {
            auto[l, r] = ranges.top();
            ranges.pop();
            if (r <= l) continue;
            auto pos = onegin::partition(l, r, cmp);
            if (std::distance(l, pos) > std::distance(pos, r)) {
                ranges.emplace(l, pos);
                ranges.emplace(pos + 1, r);
            } else {
                ranges.emplace(pos + 1, r);
                ranges.emplace(l, pos);
            }
        }
    }

    template<class RandomIt>
    void sort(RandomIt first, RandomIt last) {
        onegin::sort(first, last, std::less<typename std::iterator_traits<RandomIt>::value_type>());
    }
}