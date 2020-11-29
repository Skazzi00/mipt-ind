#pragma once

#include <algorithm>
#include <utility>
#include <string>
#include <cstdio>
#include <unistd.h>

#include "string_view.hpp"
#include "vector.hpp"
#include "string_pool.hpp"
#include "algo.hpp"
#include "io.hpp"

namespace mipt {

class DecisionTree {
  struct Node {
    Node *left;
    Node *right;
    StringPool::StringView data;

    static Node ctor() {
      Node res{};
      pCtor(&res);
      return res;
    }

    static void pCtor(Node *node) {
      node->left = nullptr;
      node->right = nullptr;
      node->data = StringPool::StringView::ctor();
    }

    static Node *New() {
      Node *res = static_cast<Node *>(calloc(1, sizeof(Node)));
      pCtor(res);
      return res;
    }

    void dtor() {
      if (left) {
        left->dtor();
        free(left);
        left = nullptr;
      }

      if (right) {
        right->dtor();
        free(right);
        right = nullptr;
      }
    }
  };

  Node root;
  StringPool::Allocator *stringAllocator;

  static void skipWhitespace(const char *&it, const char *end) {
    while (it != end && isspace(*it)) it++;
  }

  static const char *parseExpression(const char *input,
                                     const char *end,
                                     Node *node,
                                     StringPool::Allocator *poolAllocator) {
    auto firstChar = input;
    skipWhitespace(firstChar, end);

    auto endOfName = find(firstChar + 1, end, '"');

    node->data = poolAllocator->allocString(firstChar + 1, endOfName);

    auto it = endOfName + 1;
    skipWhitespace(it, end);

    if (it == end) {
      return it;
    }

    if (*it == '[') {
      node->left = Node::New();
      node->right = Node::New();
      it = parseExpression(it + 1, end, node->left, poolAllocator);
      it = parseExpression(it, end, node->right, poolAllocator);
      skipWhitespace(it, end);

      if (*it == ']') {
        return it + 1;
      }
    }
    return it;
  }

  void serializeNode(FILE *fp, Node *node) {
    fprintf(fp, "\"%s\"", node->data.c_str());
    if (node->left && node->right) {
      fprintf(fp, " [ ");
      serializeNode(fp, node->left);
      fprintf(fp, " ");
      serializeNode(fp, node->right);
      fprintf(fp, " ]");
    }
  }

  void dumpNode(FILE *out, Node *node) {
    if (!node)
      return;
    if (node->left == nullptr && node->right == nullptr)
      fprintf(out,
              "\tnode_%p "
              "[label=\"%s\" fillcolor=\"aliceblue\" style=filled];\n",
              (void *) node,
              node->data.c_str());
    else
      fprintf(out, "\tnode_%p "
                   "[label=\"%s\"];\n", (void *) node, node->data.c_str());
    if (node->left)
      fprintf(out,
              "\tnode_%p->node_%p "
              "[color=\"green\" label=\"yes\" fontcolor=\"green\"];\n",
              (void *) node,
              (void *) node->left);
    if (node->right)
      fprintf(out,
              "\tnode_%p->node_%p "
              "[color=\"darkred\" label=\"no\" fontcolor=\"darkred\"];\n",
              (void *) node,
              (void *) node->right);
    dumpNode(out, node->left);
    dumpNode(out, node->right);
  }

  struct Fact {
    string_view data;
    bool isTrue;

    static Fact ctor(const string_view &data, bool isTrue) {
      Fact res;
      res.data = data;
      res.isTrue = isTrue;
      return res;
    }

    bool equal(const Fact &other) {
      return data == other.data && isTrue == other.isTrue;
    }
  };

  bool findFacts(string_view name, Node *node, Vector<Fact> &facts) {
    if (node->left == nullptr && node->right == nullptr) {
      return strcmp(name, node->data.c_str()) == 0;
    }

    bool leftChild = findFacts(name, node->left, facts);
    bool rightChild = findFacts(name, node->right, facts);
    if (leftChild) {
      facts.push_back(Fact::ctor(node->data.c_str(), true));
    }
    if (rightChild) {
      facts.push_back(Fact::ctor(node->data.c_str(), false));
    }

    return leftChild || rightChild;
  }

  void addName(Node *curNode) const {
    char name[128] = "";
    char diff[128] = "";
    input(name, 127, "Enter name of what you guessed:\n");

    input(diff, 127, "Enter fact which true for %s and false for %s?\n", name, curNode->data.c_str());

    curNode->left = Node::New();
    curNode->right = Node::New();
    curNode->right->data = curNode->data;
    curNode->left->data = stringAllocator->allocString(name);
    curNode->data = stringAllocator->allocString(diff);
  }

  Node *findUserNode() {
    Node *curNode = &root;
    while (curNode->left != nullptr && curNode->right != nullptr) {
      char ans[20] = "yes";
      input(ans, 19, "%s? [y/n]\n", curNode->data.c_str());
      if (ans[0] == 'y') {
        curNode = curNode->left;
      } else if (ans[0] == 'n') {
        curNode = curNode->right;
      }
    }
    return curNode;
  }

  void printFact(const Fact &fact, const char *separator) {
    if (fact.isTrue) {
      sayAndPrintf("%s%s", fact.data.data(), separator);
    } else {
      sayAndPrintf("not %s%s", fact.data.data(), separator);
    }
  }

  void printFacts(const Vector<Fact> &facts, size_t start, size_t end) {
    if (end == start) {
      return;
    }
    for (size_t i = start; i + 1 < end; ++i) {
      printFact(facts[i], ", ");
    }
    printFact(facts[end - 1], ".\n");
  }

 public:
  static DecisionTree ctor(string_view data, StringPool::Allocator *poolAllocator) {
    DecisionTree res{};
    res.root = Node::ctor();
    res.stringAllocator = poolAllocator;
    parseExpression(data.begin(), data.end(), &res.root, poolAllocator);
    return res;
  }

  void dtor() {
    root.dtor();
  }

  void serialize(FILE *fp) {
    serializeNode(fp, &root);
  }

  void guess() {
    Node *curNode = findUserNode();

    char ans[20] = "yes";
    input(ans, 19, "Is it %s? [y/n]\n", curNode->data.c_str());

    if (ans[0] == 'y') {
      sayAndPrintf("EZ\n");
    } else if (ans[0] == 'n') {
      addName(curNode);
    }

  }

  void enterNameAndGetDef() {
    char name[128] = "";
    input(name, 127, "Enter name\n");

    getDefenition(name);
  }

  void getDefenition(const char *name) {
    Vector<Fact> facts{};
    findFacts(name, &root, facts);
    if (facts.empty()) {
      sayAndPrintf("I know nothing about %s\n", name);
      return;
    }

    sayAndPrintf("%s ", name);
    for (size_t i = 0; i + 1 < facts.size(); i++) {
      printFact(facts[i], ", ");
    }
    printFact(facts.back(), ".\n");
  }

  void enterNamesAndGetDiff() {
    char name1[128] = "";
    char name2[128] = "";
    input(name1, 127, "Enter first name\n");
    input(name2, 127, "Enter second name\n");
    getDifference(name1, name2);
  }

  void getDifference(const char *name1, const char *name2) {
    Vector<Fact> facts1{};
    Vector<Fact> facts2{};

    findFacts(name1, &root, facts1);
    findFacts(name2, &root, facts2);

    if (facts1.empty()) {
      printf("I know nothing about %s\n", name1);
      return;
    }

    if (facts2.empty()) {
      printf("I know nothing about %s\n", name2);
      return;
    }

    reverse(facts1.begin(), facts1.end());
    reverse(facts2.begin(), facts2.end());

    size_t numberOfEqual = facts1.size();
    for (size_t i = 0; i < facts1.size() && i < facts2.size(); ++i) {
      if (!facts1[i].equal(facts2[i])) {
        numberOfEqual = i;
        break;
      }
    }

    if (numberOfEqual == 0) {
      sayAndPrintf("Objects have nothing similar.\n");
    } else {
      sayAndPrintf("Both objects ");
      printFacts(facts1, 0, numberOfEqual);
    }

    if (numberOfEqual == facts1.size() && facts1.size() == facts2.size()) {
      sayAndPrintf("%s is %s.", name1, name2);
    } else {
      sayAndPrintf("%s ", name1);
      printFacts(facts1, numberOfEqual, facts1.size());

      sayAndPrintf("%s ", name2);
      printFacts(facts2, numberOfEqual, facts2.size());
    }
  }

  void dump() {
    FILE *fp = fopen("akinator.gv", "w");
    fprintf(fp, "digraph akinator {\n");
    dumpNode(fp, &root);
    fprintf(fp, "}\n");
    fclose(fp);
    system("dot -Tsvg akinator.gv -o akinator.svg");
  }
};
}