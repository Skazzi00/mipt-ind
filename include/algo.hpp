#pragma once

#include "cpputils.hpp"

namespace mipt {

template<class Iter, class T>
Iter find(Iter first, Iter last, const T &value) {
  for (; first != last; ++first) {
    if (*first == value) {
      return first;
    }
  }
  return last;
}

template<class Iter>
void reverse(Iter first, Iter last) {
  while ((first != last) && (first != --last)) {
    iswap(first++, last);
  }
}

}