#include "list.hpp"
#include "gtest/gtest.h"
#include "base_test.hpp"

using namespace mipt;


TEST(ListCorrectness, insert_begin) {
  size_t const N = 20;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();

    for (size_t i = 0; i != N; ++i) a.insert(a.begin(), i);

    for (size_t i = 0; i != N / 2; ++i) {
      EXPECT_EQ(i, a.getByIndex(a.prev(a.end())));
      a.erase(a.prev(a.end()));
    }

    a.dtor();
  }
  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, insert_end) {
  size_t const N = 20;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();

    for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);
    EXPECT_EQ(N, a.size());

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(N + i, a.size());
      a.insert(a.end(), 4 * i + 1);
      EXPECT_EQ(4 * i + 1, a.back());
    }

    a.optimize();
    for (size_t i = 0, it = a.begin(); i != N; ++i, it++) EXPECT_EQ(2 * i + 1, a.getByIndex(it));
    a.dtor();

  }
  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, erase_end) {
  size_t const N = 500;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();

    for (size_t i = 0; i != 2 * N; ++i) a.push_back(2 * i + 1);

    for (size_t i = 0; i != N; ++i) a.erase(a.prev(a.end()));

    for (size_t i = 0, it = a.begin(); i != N; ++i, ++it) EXPECT_EQ(2 * i + 1, a.getByIndex(it));
    a.dtor();

  }
  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, erase_begin) {
  size_t const N = 500;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();

    for (size_t i = 0; i != 2 * N; ++i) a.push_back(2 * i + 1);

    for (size_t i = 0; i != N; ++i) a.erase(a.begin());

    a.insert(a.begin(), 1);
    EXPECT_TRUE(a.isOptimized());
    a.pop_front();
    for (size_t i = 0, it = a.begin(); i != N; ++i, it++) EXPECT_EQ(2 * (i + N) + 1, a.getByIndex(it));
    a.dtor();

  }
  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, erase) {
  size_t const N = 100;
  {
    for (size_t i = 0; i != N; ++i) {
      List<element<size_t> > a = List<element<size_t >>::ctor();
      for (size_t j = 0; j != N; ++j) a.push_back(2 * j + 1);

      auto it = a.begin();
      for (size_t j = 0; j < i; ++j) it = a.next(it);
      a.erase(it);
      size_t cnt = 0;
      a.optimize();
      for (size_t j = 0, iter = a.begin(); j != N - 1; ++j, iter++) {
        if (j == i) ++cnt;
        EXPECT_EQ(2 * cnt + 1, a.getByIndex(iter));
        ++cnt;
      }
      a.dtor();

    }
  }
  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, pop_back) {
  size_t const N = 500;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();

    for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);

    for (size_t i = N; i != 0; --i) {
      EXPECT_EQ(2 * i - 1, a.back());
      EXPECT_EQ(i, a.size());
      a.pop_back();
    }
    EXPECT_TRUE(a.empty());
    a.dtor();
  }
  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, front_back) {
  size_t const N = 500;
  List<element<size_t> > a = List<element<size_t >>::ctor();
  for (size_t i = 0; i != N; ++i) a.push_back(2 * i + 1);

  EXPECT_EQ(1, a.front());

  EXPECT_EQ(999, a.back());
  a.dtor();

}

TEST(ListCorrectness, push_back) {
  size_t const N = 5000;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();
    for (size_t i = 0; i != N; ++i)
      a.push_back(i);

    a.optimize();

    for (size_t i = 0, it = a.begin(); i != N; ++i, it++)
      EXPECT_EQ(i, a.getByIndex(it));
    a.dtor();

  }

  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, push_front) {
  size_t const N = 5000;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();
    for (size_t i = 0; i != N; ++i)
      a.push_front(i);

    a.optimize();
    for (size_t i = 0, it = a.begin(); i != N; ++i, it++)
      EXPECT_EQ(N - 1 - i, a.getByIndex(it));
    a.dtor();

  }

  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, optimize) {
  size_t const N = 5000;
  {
    List<element<size_t> > a = List<element<size_t >>::ctor();
    for (size_t i = 0; i != N; ++i)
      a.push_back(i);

    for (size_t i = 0; i != N / 2; ++i)
      a.pop_front();

    for (size_t i = 0, it = a.begin(); i != N / 2; ++i, ++it)
      EXPECT_EQ(i + N / 2, a.getByIndex(it));
    a.dtor();

  }

  element<size_t>::expect_no_instances();
}

TEST(ListCorrectness, middle) {
  size_t const N = 17;
  {
    List<int> a = List<int>::ctor();

    for (int i = 0; i != N; ++i) {
      a.push_back(i);
    }

    a.insert(N / 2 + 2, -1);
    a.insert(N / 2 + 1, -2);
    a.pop_back();
    a.pop_back();

    FILE *fp = fopen("listdp.html", "w");
    a.dump(fp);
    fclose(fp);

    a.dtor();
  }
}

TEST(ListCorrectness, begin_end_push) {
  List<int> a = List<int>::ctor();
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);
  a.pop_front();
  a.pop_back();
  a.pop_front();

  a.push_front(6);
  a.push_back(7);
  a.push_front(8);

  EXPECT_TRUE(a.isOptimized());

  FILE *fp = fopen("beginend.html", "w");
  a.dump(fp);
  fclose(fp);

  a.dtor();
}
