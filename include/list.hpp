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
    res.nodes[0].deleted = false;
    for (size_t i = 1; i < MIN_SIZE; ++i) {
      res.nodes[i].next = i + 1;
      res.nodes[i].deleted = true;
    }
    res.size_ = 1;
    res.free = 1;
    res.optimized = true;
    return res;
  }

  void cut(size_t pos) {
    if (nodes[pos].next != static_cast<size_t>(-1))
      nodes[nodes[pos].next].prev = nodes[pos].prev;

    if (nodes[pos].prev != static_cast<size_t>(-1))
      nodes[nodes[pos].prev].next = nodes[pos].next;
    else
      free = nodes[pos].next;

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
      if (next(free) < nodes.size())
        nodes[next(free)].prev = static_cast<size_t>(-1);
      free = next(free);
      nodes[res].deleted = false;
      return res;
    }
  }

  void addToFree(size_t pos) {
    nodes[pos].deleted = true;
    linkNodes(pos, free);
    nodes[pos].prev = static_cast<size_t>(-1);
    free = pos;
  }

  T &getByIndex(size_t index) {
    return nodes[index].value;
  }

  /**
   * Insert item before index.
   * If insert at end or begin (if was place left from linear block) of list, list remains optimized.
   */
  size_t insert(size_t index, const T &value) {
    bool prevOptimized = optimized;
    const bool isBeginInsertOptimized = begin() != 0 && optimized && index == begin() && nodes[begin() - 1].deleted;
    const bool isEndInsert = end() == index;
    size_t freeIndex = 0;
    if (isBeginInsertOptimized) {
      cut(begin() - 1);
      freeIndex = begin() - 1;
      nodes[freeIndex].deleted = false;
    } else {
      freeIndex = getFreeIndex();
    }
    nodes[freeIndex].value = value;
    linkNodes(prev(index), freeIndex);
    linkNodes(freeIndex, index);
    incSize();
    if (isEndInsert || isBeginInsertOptimized) {
      optimized = prevOptimized;
    }
    return freeIndex;
  }

  /**
   * Doesn't change optimized state
   */
  size_t push_back(const T &value) {
    return insert(end(), value);
  }

  /**
   * Doesn't change optimized state
   */
  size_t push_front(const T &value) {
    return insert(begin(), value);
  }

  /**
    * If erased from end or begin, optimized state doesnt change
    */
  void erase(size_t index) {
    const bool prevOptimized = optimized;
    const bool isBeginErase = index == begin();
    const bool isEndErase = prev(end()) == index;
    decSize();
    cut(index);
    nodes[index].deleted = true;
    addToFree(index);
    if (isBeginErase || isEndErase) {
      optimized = prevOptimized;
    }
  }
  /**
   * Doesn't change optimized state
   */
  void pop_front() {
    erase(begin());
  }

  /**
   * Doesn't change optimized state
   */
  void pop_back() {
    erase(prev(end()));
  }

  /**
   * If returned 0, then logic index incorrect
   */
  size_t slowLinearSearchFromLogicalToPhysicalIndex(size_t logicIndex) {
    if (optimized) {
      return begin() + logicIndex;
    }
    if (logicIndex >= size()) {
      return 0;
    }
    size_t phAddress = begin();
    for (size_t i = 0; i < logicIndex; i++) {
      phAddress = next(phAddress);
    }
    return phAddress;
  }

  bool isOptimized() {
    return optimized;
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
    if (1 != nodes.size()) {
      fprintf(fp, "\"node%d\"->\"node%lu\"[style=invis weight=10000];\n", 0, linear ? 1 : next(0));
    }

    size_t start = linear ? 1 : begin();
    size_t end = linear ? nodes.size() : this->end();
    size_t logicCnt = 1;

    for (size_t index = start; index != end; linear ? ++index : index = next(index)) {
      fprintf(fp,
              "\"node%ld\" [label = \"<index> %ld | <prev> prev | <next> next | <val> value = ",
              index,
              linear ? index : logicCnt++);
      printToFile(fp, nodes[index].value);
      fprintf(fp, "\"");

      if (nodes[index].deleted)
        fprintf(fp, "fillcolor=%s style=filled", FreeNodeColor);

      fprintf(fp, "];\n");

      if (index != (linear ? nodes.size() - 1 : prev(end))) {
        fprintf(fp, "\"node%ld\"->\"node%ld\"[style=invis weight=100000];\n", index, linear ? index + 1 : next(index));
      }
    }

    if (!linear) {
      for (size_t i = 1; i < nodes.size(); ++i) {
        if (!nodes[i].deleted) continue;
        fprintf(fp, "\"node%ld\" [label = \"<index> %ld | <prev> prev | <next> next | <val> value = ", i, logicCnt++);
        printToFile(fp, nodes[i].value);
        fprintf(fp, "\"fillcolor=%s style=filled];\n", FreeNodeColor);
      }
    }
  }

  void dumpEdges(FILE *fp) {
    for (size_t nodeIndex = begin(), i = 0; i < size_; nodeIndex = next(nodeIndex), i++) {
      const size_t prevIndex = prev(nodeIndex);
      const size_t nextIndex = next(nodeIndex);

      fprintf(fp, "\"node%ld\":prev->\"node%ld\"[weight=0 %s];\n", nodeIndex, prevIndex, "constraint=false");
      fprintf(fp, "\"node%ld\":next->\"node%ld\"[weight=0 %s];\n", nodeIndex, nextIndex, "constraint=false");
    }
  }

  void dumpEnd(FILE *fp) { fprintf(fp, "}\n"); }

  void dumpMeta(FILE *fp) {
    fprintf(fp,
            "\"meta\" [label = \"capacity = %zu | size = %zu | free = %zu\" fillcolor=pink style=filled];\n",
            nodes.size(),
            size(),
            free);
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

  void dumpFields(FILE *fp) {
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
    dumpEdges(fp);
    dumpFree(fp);
    dumpEnd(fp);
  }

  void dump(FILE *fp) {
    static int tmpFileCnt = 0;
    static const int CmdSize = 128;
    fprintf(fp, "<pre><code>\n");

    fprintf(fp, "List&#x3C;%s&#x3E; [0x%zX] {\n", typeid(T).name(), reinterpret_cast<size_t>(this));

    dumpFields(fp);

    fprintf(fp, "}\n");
    fprintf(fp, "</code></pre>\n");

    char cmd[CmdSize] = "";

    FILE *gvTmpFile = fopen("gvtemp.gv", "w");
    dumpGraph(gvTmpFile, true);
    fclose(gvTmpFile);
    sprintf(cmd, "dot -Tpng gvtemp.gv -o gvtemp%d.png", ++tmpFileCnt);
    fprintf(fp, "<h1>Physical Order</h1>\n<img src=gvtemp%d.png width=100%%>\n", (int) tmpFileCnt);
    system(cmd);

    gvTmpFile = fopen("gvtemp.gv", "w");
    dumpGraph(gvTmpFile, false);
    fclose(gvTmpFile);
    sprintf(cmd, "dot -Tpng gvtemp.gv -o gvtemp%d.png", ++tmpFileCnt);
    fprintf(fp, "<h1>Logical Order</h1>\n<img src=gvtemp%d.png width=100%%>\n", (int) tmpFileCnt);

    system(cmd);
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