#include "list.hpp"
#include "gtest/gtest.h"
#include "base_test.hpp"
#include <unordered_set>

using namespace mipt;


TEST(ListCorrectness, push_back) {
    size_t const N = 5000;
    {
        List<element<size_t> > a;
        for (size_t i = 0; i != N; ++i)
            a.push_back(i);

        for (size_t i = 0; i != N; ++i)
            EXPECT_EQ(i, *a.get(i));
    }

    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, push_front) {
    size_t const N = 5000;
    {
        List<element<size_t> > a;
        for (size_t i = 0; i != N; ++i)
            a.push_front(i);

        for (size_t i = 0; i != N; ++i)
            EXPECT_EQ(N - 1 - i, *a.get(i));
    }

    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, push_back_from_self) {
    size_t const N = 500;
    {
        List<element<size_t> > a;
        a.push_back(42);
        for (size_t i = 0; i != N; ++i)
            a.push_back(*a.get(0));

        for (size_t i = 0; i != a.size(); ++i) EXPECT_EQ(42, *a.get(i));
    }

    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, subscription) {
    size_t const N = 500;
    List<size_t> a;
    for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);

    for (size_t i = 0; i != N; ++i) EXPECT_EQ(2 * i + 1, *a.get(i));

    List<size_t> const &ca = a;

    for (size_t i = 0; i != N; ++i) EXPECT_EQ(2 * i + 1, *ca.get(i));

}


TEST(ListCorrectness, front_back) {
    size_t const N = 500;
    List<element<size_t> > a;
    for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);

    EXPECT_EQ(1, a.front());
    EXPECT_EQ(1, as_const(a).front());

    EXPECT_EQ(999, a.back());
    EXPECT_EQ(999, as_const(a).back());
}


TEST(ListCorrectness, copy_ctor) {
    size_t const N = 100;
    {
        List<element<size_t> > a;
        for (size_t i = 0; i != N; ++i) a.push_back(i);

        List<element<size_t> > b = a;
        for (size_t i = 0; i != N; ++i) EXPECT_EQ(i, *b.get(i));
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, assignment_operator) {
    size_t const N = 500;
    {
        List<element<size_t> > a;
        for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);

        List<element<size_t> > b;
        b.push_back(42);

        b = a;
        EXPECT_EQ(N, b.size());
        for (size_t i = 0; i != N; ++i) {
            auto tmp = *b.get(i);
            EXPECT_EQ(2 * i + 1, tmp);
        }
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, self_assignment) {
    size_t const N = 500;
    {
        List<element<size_t> > a;
        for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign-overloaded"
        a = a;
#pragma clang diagnostic pop

        for (size_t i = 0; i != N; ++i) EXPECT_EQ(2 * i + 1, *a.get(i));
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, pop_back) {
    size_t const N = 500;
    {
        List<element<size_t> > a;

        for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);

        for (size_t i = N; i != 0; --i) {
            EXPECT_EQ(2 * i - 1, a.back());
            EXPECT_EQ(i, a.size());
            a.pop_back();
        }
        EXPECT_TRUE(a.empty());
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, emplace_begin) {
    size_t const N = 500;
    {
        List<element<size_t> > a;

        for (size_t i = 0; i != N; ++i) a.emplace(a.begin(), i);

        for (size_t i = 0; i != N; ++i) {
            EXPECT_EQ(i, a.back());
            a.pop_back();
        }
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, emplace_end) {
    size_t const N = 500;
    {
        List<element<size_t> > a;

        for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);
        EXPECT_EQ(N, a.size());

        for (size_t i = 0; i != N; ++i) {
            EXPECT_EQ(N + i, a.size());
            a.emplace(a.end(), 4 * i + 1);
            EXPECT_EQ(4 * i + 1, a.back());
        }

        for (size_t i = 0; i != N; ++i) EXPECT_EQ(2 * i + 1, *a.get(i));
    }
    element<size_t>::expect_no_instances();
}


TEST(ListCorrectness, erase) {
    size_t const N = 500;
    {
        for (size_t i = 0; i != N; ++i) {
            List<element<size_t>> a;
            for (size_t j = 0; j != N; ++j) a.push_back(2 * j + 1);

            auto it = a.begin();
            std::advance(it, static_cast<long>(i));
            a.erase(it);
            size_t cnt = 0;
            for (size_t j = 0; j != N - 1; ++j) {
                if (j == i) ++cnt;
                EXPECT_EQ(2 * cnt + 1, *a.get(j));
                ++cnt;
            }
        }
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, erase_begin) {
    size_t const N = 500;
    {
        List<element<size_t> > a;

        for (size_t i = 0; i != 2 * N; ++i) a.push_back(2 * i + 1);

        for (size_t i = 0; i != N; ++i) a.erase(a.begin());

        for (size_t i = 0; i != N; ++i) EXPECT_EQ(2 * (i + N) + 1, *a.get(i));
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, erase_end) {
    size_t const N = 500;
    {
        List<element<size_t>> a;

        for (size_t i = 0; i != 2 * N; ++i) a.push_back(2 * i + 1);


        for (size_t i = 0; i != N; ++i) a.erase(--a.end());

        for (size_t i = 0; i != N; ++i) EXPECT_EQ(2 * i + 1, *a.get(i));
    }
    element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, optimize) {
    size_t const N = 5000;
    {
        List<element<size_t> > a;
        for (size_t i = 0; i != N; ++i)
            a.push_back(i);

        for (size_t i = 0; i != N / 2; ++i)
            a.pop_front();

        a.optimize();

        for (size_t i = N / 2; i != N; ++i)
            EXPECT_EQ(i, *a.get(i - N / 2));
    }

    element<size_t>::expect_no_instances();
}