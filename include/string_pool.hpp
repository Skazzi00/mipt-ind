#pragma once

#include <utility>
#include <cstring>
#include <vector>
#include <set>
#include "vector.hpp"

namespace mipt::StringPool {

class Allocator;

class StringView {
 public:

  static StringView ctor() {
    StringView res;
    return res;
  }

  const char *c_str() const noexcept {
    if (!empty()) {
      return mPtr;
    } else {
      return &mNil;
    }
  }

  size_t length() const noexcept {
    return mLength;
  }

  bool empty() const noexcept {
    return mLength == 0;
  }

  // +1 : this > other
  //  0 : this == other
  // -1 : this < other
  int compare(const StringView &other) const noexcept {
    const size_t minLength = mLength < other.mLength ?
                             mLength : other.mLength;

    const int result = strncmp(mPtr, other.mPtr, minLength);

    if (result != 0)
      return result;

    if (mLength < other.mLength)
      return -1;

    if (mLength > other.mLength)
      return 1;

    return 0;
  }

  friend class Allocator;

  friend void swap(StringView &a, StringView &b) noexcept {
    using std::swap;
    swap(a.mPtr, b.mPtr);
    swap(a.mLength, b.mLength);
  }

 private:
  const char *mPtr{};
  size_t mLength{};

  char mNil{};

  static StringView ctor(const char *ptr, size_t length) noexcept {
    StringView res;
    res.mPtr = ptr;
    res.mLength = length;
    return res;
  }
};

class Allocator {
 public:
  static Allocator ctor() {
    Allocator res;
    return res;
  }

  void dtor() noexcept {
    clear();
  }


  void clear() {
    for (auto &pHeader : mBlocks) {
      Free(pHeader);
      pHeader = nullptr;
    }

    mBlocks.clear();

    mpNext = nullptr;
    mpEnd = nullptr;
  }

  StringView allocString(const char *ptr) {
    return allocString(ptr, ptr + strlen(ptr));
  }

  // return empty String if string to big, or not enough memory
  StringView allocString(const char *start, const char *end) {
    const size_t length = static_cast<const size_t>(end - start);
    const size_t lengthWithNull = length + 1;
    char *ptr = allocMem(lengthWithNull);
    memcpy(ptr, start, length);
    ptr[length] = '\0';
    return StringView::ctor(ptr, length);
  }

 private:

  struct Header {
    size_t size;
    // after header placed char array
  };

  enum {
    minBlockSize = 600'000,
    maxStringLength = 1024 * 1024
  };

  Vector<Header *> mBlocks{};

  char *mpNext{};
  char *mpEnd{};

  static void *Alloc(size_t size) noexcept {
    return malloc(size);
  }

  static void Free(void *ptr) noexcept {
    return free(ptr);
  }

  // return nullptr if length to big
  char *allocMem(size_t length) {
    if (length > maxStringLength) {
      return nullptr;
    }

    if (mpNext + length <= mpEnd) {
      char *res = mpNext;
      mpNext += length;
      return res;
    }

    size_t blockSize = length + sizeof(Header);
    if (blockSize < minBlockSize) {
      blockSize = minBlockSize;
    }

    char *pBlockStart = static_cast<char *>(Alloc(blockSize));
    if (pBlockStart == nullptr) {
      return nullptr;
    }

    mpEnd = pBlockStart + blockSize;

    Header *pHeader = reinterpret_cast<Header *>(pBlockStart);
    pHeader->size = blockSize;

    mpNext = reinterpret_cast<char *>(pHeader + 1);

    mBlocks.push_back(pHeader);

    return allocMem(length);
  }
};

} // namespace mipt