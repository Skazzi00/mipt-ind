#pragma once

#include <utility>
#include <cstring>
#include <vector>
#include <set>
#include "vector.hpp"

namespace mipt::StringPool {

class Allocator;

/**
 * A representation of string allocated by StringPool::Allocator.
 * Doesn't own a string.
 */
class StringView {
 public:
  StringView() {}

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

  /**
   *
   * @param other string to compare with
   * @return
   * +1 : this > other <br>
   *  0 : this == other <br>
   * -1 : this < other
   */
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

  StringView(const char *ptr, size_t length) noexcept: mPtr(ptr), mLength(length) {}
};

/**
 * Contains a vector of large blocks in which it allocates space for new strings.
 * If the space in the blocks runs out, then a new block is requested.
 */
class Allocator {
 public:

  Allocator() {}

  ~Allocator() noexcept {
    clear();
  }

  /**
   * Free all blocks
   */
  void clear() noexcept{
    for (auto &pHeader : mBlocks) {
      Free(pHeader);
      pHeader = nullptr;
    }

    mBlocks.clear();

    mpNext = nullptr;
    mpEnd = nullptr;
  }

  /**
   *
   * @param ptr c-string to alloc
   * @return StringView to copied string to allocator's memory.
   */
  StringView allocString(const char *ptr) {
    return allocString(ptr, ptr + strlen(ptr));
  }

  /**
   * @param start of string
   * @param end of string
   * @return empty String if string to big, or not enough memory
   */
  StringView allocString(const char *start, const char *end) {
    const size_t length = static_cast<const size_t>(end - start);
    const size_t lengthWithNull = length + 1;
    char *ptr = allocMem(lengthWithNull);
    memcpy(ptr, start, length);
    ptr[length] = '\0';
    return StringView(ptr, length);
  }

 private:

  struct Header {
    size_t size;
    // after header placed char array
  };

  enum {
    // Allocated blocks must be minimum this size. If string length greater, then allocates a block of string size
    BlockSize = 512 * 1024,
    // Strings larger then MaxStringLength will not be allocated
    MaxStringLength = BlockSize * 2
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

  /**
   * Finds the required memory in current block,
   * if there is not enough memory in the current block,
   * then allocates a new large block. This block becomes current.
   *
   * @param length required memory size
   * @return pointer to allocated memory or nullptr if length to big
   */
  char *allocMem(size_t length) {
    if (length > MaxStringLength) {
      return nullptr;
    }

    if (mpNext + length <= mpEnd) {
      char *res = mpNext;
      mpNext += length;
      return res;
    }

    size_t allocatedBlockSize = length + sizeof(Header);
    if (allocatedBlockSize < BlockSize) {
      allocatedBlockSize = BlockSize;
    }

    char *pBlockStart = static_cast<char *>(Alloc(allocatedBlockSize));
    if (pBlockStart == nullptr) {
      return nullptr;
    }

    mpEnd = pBlockStart + allocatedBlockSize;

    Header *pHeader = reinterpret_cast<Header *>(pBlockStart);
    pHeader->size = allocatedBlockSize;

    mpNext = reinterpret_cast<char *>(pHeader + 1);

    mBlocks.push_back(pHeader);

    return allocMem(length);
  }
};

} // namespace mipt