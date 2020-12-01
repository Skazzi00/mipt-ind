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
      res.nodes[i].deleted = true;
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
    nodes[pos].deleted = true;
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

  /**
   * If returned 0, then logic index incorrect
   */
  size_t slowLinearSearchFromLogicalToPhysicalIndex(size_t logicIndex) {
    if (logicIndex >= size()) {
      return 0;
    }
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

 private:
  static constexpr const char *FreeNodeColor = "green";
  static constexpr const char *HeadNodeColor = "lightblue";

  static void dumpHeader(FILE *fp) {
    fprintf(fp, "digraph list {\n"
                "graph [rankdir = \"LR\"];\n"
                "rank = same;\n"
                "node[shape = record];\n");
  }

  void dumpNodes(FILE *fp, bool linear) {
    fprintf(fp,
            "\"node0\" [label = \"<index> HEAD | <prev> prev | <next> next\" fillcolor=%s style=filled];\n",
            HeadNodeColor);
    if (linear && 1 != nodes.size()) {
      fprintf(fp, "\"node%d\"->\"node%d\"[style=invis weight=10000];\n", 0, 1);
    }

    for (size_t index = 1; index < nodes.size(); ++index) {
      fprintf(fp, "\"node%ld\" [label = \"<index> %ld | <prev> prev | <next> next | <val> value = ", index, index);
      printToFile(fp, nodes[index].value);
      fprintf(fp, "\"");

      if (nodes[index].deleted)
        fprintf(fp, "fillcolor=%s style=filled", FreeNodeColor);

      fprintf(fp, "];\n");

      if (linear && index != nodes.size() - 1) {
        fprintf(fp, "\"node%ld\"->\"node%ld\"[style=invis weight=100000];\n", index, index + 1);
      }
    }
  }

  void dumpEdges(FILE *fp, bool linear) {
    for (size_t nodeIndex = begin(), i = 0; i < size_; nodeIndex = next(nodeIndex), i++) {
      const size_t prevIndex = prev(nodeIndex);
      const size_t nextIndex = next(nodeIndex);

      fprintf(fp,
              "\"node%ld\":prev->\"node%ld\":index[weight=0 %s];\n",
              nodeIndex,
              prevIndex,
              linear ? "constraint=false" : "");
      fprintf(fp,
              "\"node%ld\":next->\"node%ld\":index[weight=0 %s];\n",
              nodeIndex,
              nextIndex,
              linear ? "constraint=false" : "");
    }
  }

  void dumpEnd(FILE *fp) { fprintf(fp, "}\n"); }

  void dumpMeta(FILE *fp) {
    fprintf(fp, "\"meta\" [label = \"capacity = %zu | size = %zu | free = %zu\" fillcolor=pink style=filled];\n", nodes.size(), size(), free);
  }

  void dumpFree(FILE *fp) {
    fprintf(fp, "\"free\" [ label = \"free\" fillcolor=lightgreen style=filled];\n");

    if (size_ != nodes.size()) {
      fprintf(fp, "\"free\"->\"node%ld\";\n", free);
      size_t curIndex = free;
      for (size_t i = 0; i + size_ + 1 < nodes.size(); ++i, curIndex = next(curIndex)) {
        size_t nextIndex = next(curIndex);
        fprintf(fp, "\"node%ld\":next->\"node%ld\";\n", curIndex, nextIndex);
      }
    }
  }

  void dumpFields( FILE *fp) {
    fprintf(fp, "\tnodes     = [%p]\n", static_cast<void *>(nodes.data()));
    fprintf(fp, "\tsize_     = %zu\n", size_);
    fprintf(fp, "\tfree      = %zu\n", free);
    fprintf(fp, "\tcapacity  = %zu\n", nodes.capacity());
    fprintf(fp, "\toptimized = %s\n", optimized ? "true" : "false");
  }

 public:

  void dumpGraph(FILE *fp, bool linear) {
    dumpHeader(fp);
    dumpMeta(fp);
    dumpNodes(fp, linear);
    dumpEdges(fp, linear);
    dumpFree(fp);
    dumpEnd(fp);
  }
  void dump(FILE *fp) {
    fprintf(fp, "<pre><code>\n");

    fprintf(fp, "List&#x3C;%s&#x3E; [0x%zX] {\n", typeid(T).name(), reinterpret_cast<size_t>(this));

    dumpFields(fp);

    fprintf(fp, "}\n");

    FILE *gvTmpFile = fopen("gvtemp.gv", "w");
    dumpGraph(gvTmpFile, true);
    fclose(gvTmpFile);
    system("dot -Tpng gvtemp.gv -o gvtempl.png");

    gvTmpFile = fopen("gvtemp.gv", "w");
    dumpGraph(gvTmpFile, false);
    fclose(gvTmpFile);
    system("dot -Tpng gvtemp.gv -o gvtemp.png");

    fprintf(fp, "</code></pre>\n");
    fprintf(fp, "<img src=gvtemp.png width=100%%>\n");
    fprintf(fp, "<img src=gvtempl.png width=100%%>\n");

  }

  Status status() {
    const Status vecCode = validate(nodes);
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