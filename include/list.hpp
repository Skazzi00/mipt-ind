#pragma once

#include <utility>
#include <cstdio>
#include <algorithm>

#include "vector.hpp"

namespace mipt {

template<typename T>
struct List {
  static constexpr size_t MIN_SIZE = 16;

  struct Node {
    T value;
    size_t prev{};
    size_t next{};
    bool deleted{};

    static Node ctor() {
      Node res;
      return res;
    }

  };

  static List ctor() {
    List res;
    res.nodes = Vector<Node>::ctor(MIN_SIZE);
    res.nodes[0].prev = res.nodes[0].next = 0;
    for (size_t i = 1; i < MIN_SIZE; ++i) {
      res.nodes[i].next = i + 1;
    }
    res.size_ = 1;
    res.free = 1;
    res.optimized = false;
    return res;
  }

  void cut(size_t pos) {
    nodes[nodes[pos].next].prev = nodes[pos].prev;
    nodes[nodes[pos].prev].next = nodes[pos].next;
    nodes[pos].prev = pos;
    nodes[pos].next = pos;
  }

  size_t getFreeIndex() {
    if (size_ == nodes.size()) {
      nodes.push_back(Node::ctor());
      free = size_ + 1;
      nodes[size_].deleted = false;
      return size_;
    } else {
      size_t res = free;
      free = next(free);
      nodes[size_].deleted = false;
      return res;
    }
  }

  void addToFree(size_t pos) {
    linkNodes(pos, free);
    free = pos;
  }

  T &getByIndex(size_t index) {
    return nodes[index].value;
  }

  /**
   * Insert item before index
   */
  size_t insert(size_t index, const T &value) {
    size_t freeIndex = getFreeIndex();
    nodes[freeIndex].value = value;
    linkNodes(prev(index), freeIndex);
    linkNodes(freeIndex, index);
    incSize();
    return freeIndex;
  }

  size_t push_back(const T &value) {
    return insert(end(), value);
  }

  size_t push_front(const T &value) {
    return insert(begin(), value);
  }

  void erase(size_t index) {
    decSize();
    cut(index);
    nodes[index].deleted = true;
    addToFree(index);
  }

  void pop_front() {
    erase(begin());
  }

  void pop_back() {
    erase(prev(end()));
  }

  size_t slowLinearSearch(size_t logicIndex) {
    size_t phAddress = begin();
    for (size_t i = 0; i < logicIndex; i++) {
      phAddress = next(phAddress);
    }
    return phAddress;
  }

  void optimize() {
    if (optimized) return;
    Vector<Node> newNodes = Vector<Node>::ctor(size_);
    size_t curNodeIndex = begin();
    for (size_t i = 1; i < size_; ++i) {
      newNodes[i].value = nodes[curNodeIndex].value;
      curNodeIndex = next(curNodeIndex);
    }
    nodes.dtor();
    nodes = newNodes;
    for (size_t i = 0; i + 1 < size_; ++i) {
      linkNodes(i, i + 1);
    }
    linkNodes(size_ - 1, 0);
    free = size_;
    optimized = true;
  }

  size_t next(size_t index) {
    return nodes[index].next;
  }

  size_t prev(size_t index) {
    return nodes[index].prev;
  }

  void linkNodes(size_t left, size_t right) {
    nodes[left].next = right;
    nodes[right].prev = left;
  }

  void incSize() {
    optimized = false;
    size_++;
  }

  void decSize() {
    optimized = false;
    size_--;
  }

  size_t begin() {
    return nodes[0].next;
  }

  size_t end() {
    return 0;
  }

  T &back() {
    return nodes[prev(0)].value;
  }

  T &front() {
    return nodes[begin()].value;
  }

  void dump(FILE *fp, bool linear) {
    fprintf(fp, "digraph list {\n"
                "graph [rankdir = \"LR\"];\n"
                "rank = same;\n"
                "node[shape = record];\n");
    for (size_t index = 0; index < nodes.size(); ++index) {
      fprintf(fp, "\"node%ld\" [label = \"<index> %ld | <prev> prev | <next> next | <val> value = ",
              index,
              index);
      printToFile(fp, nodes[index].value);

      fprintf(fp, "\" %s];\n", nodes[index].deleted ? "fillcolor=green style=filled" : "");
      if (linear && index != nodes.size() - 1) {
        fprintf(fp, "\"node%ld\"->\"node%ld\"[style=invis weight=10000];\n", index, index + 1);
      }
    }

    for (size_t nodeIndex = begin(); nodeIndex != end(); nodeIndex = next(nodeIndex)) {
      size_t prevIndex = prev(nodeIndex);
      size_t nextIndex = next(nodeIndex);

      fprintf(fp, "\"node%ld\":prev->\"node%ld\":index;\n", nodeIndex, prevIndex);
      fprintf(fp, "\"node%ld\":next->\"node%ld\":index;\n", nodeIndex, nextIndex);
    }
    fprintf(fp, "\"node%d\":prev->\"node%ld\":index;\n", 0, prev(0));
    fprintf(fp, "\"node%d\":next->\"node%ld\":index;\n", 0, next(0));

    fprintf(fp, "\"free\" [ label = \"free\"];\n");

    if (size_ != nodes.size()) {
      fprintf(fp, "\"free\"->\"node%ld\";\n", free);
      size_t curIndex = free;
      for (size_t i = 0; i + size_ + 1 < nodes.size(); ++i, curIndex = next(curIndex)) {
        size_t nextIndex = next(curIndex);

        fprintf(fp, "\"node%ld\":next->\"node%ld\";\n", curIndex, nextIndex);
      }
    }
    fprintf(fp, "}\n");

  }

  Status status() {
    Status vecCode = validate(nodes);
    if (vecCode != Status::OK) {
      return vecCode;
    }

    size_t it = begin();
    for (int i = 0; i < size_; ++i) it = next(it);
    if (it != begin()) return Status::INVALID;

    for (int i = 0; i < size_; ++i) it = prev(it);
    if (it != begin()) return Status::INVALID;

    return Status::OK;
  }

  size_t size() {
    return size_ - 1;
  }

  bool empty() {
    return size() == 0;
  }

  void dtor() {
    nodes.dtor();
  }

 private:
  Vector<Node> nodes;
  size_t size_;
  size_t free;
  bool optimized;
};
} // namespace mipt