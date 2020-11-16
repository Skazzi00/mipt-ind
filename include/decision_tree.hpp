#pragma once

#include <algorithm>
#include <utility>
#include <string>
#include <cstdio>

#include "string_view.hpp"
#include "errors.hpp"


namespace mipt {
    class DecisionTree {
        struct Node {
            Node *left;
            Node *right;
            std::string data;

            Node() : left(nullptr), right(nullptr), data() {}

            Node(const Node &) = delete;

            Node &operator=(const Node &) = delete;

            Node(Node &&other) = delete;

            Node &operator=(Node &&) = delete;

            ~Node() {
                if (left) delete left;
                if (right) delete right;
            }
        };

        Node root;

        template<typename InputIterator>
        static InputIterator parseExpression(InputIterator input, InputIterator end, Node *node) {
            auto skipWhitespace = [&end](InputIterator &it) {
                while (it != end && isspace(*it)) it++;
            };

            auto throwFoundChar = [](char expected, char found) {
                std::string errorMessage;
                errorMessage.reserve(128);
                sprintf(errorMessage.data(), "Error occurred while parsing: Expected '%c', but found '%c'", expected,
                        found);
                throw parse_error(std::move(errorMessage));
            };

            auto throwNotFound = [](char expected) {
                std::string errorMessage;
                errorMessage.reserve(128);
                sprintf(errorMessage.data(), "Error occurred while parsing: Expected '%c', but not found", expected);
                throw parse_error(std::move(errorMessage));
            };

            auto firstChar = input;
            skipWhitespace(firstChar);

            if (firstChar == end) {
                throwNotFound('"');
            }
            if (*firstChar != '"') {
                throwFoundChar('"', *firstChar);
            }

            auto endOfName = std::find(firstChar + 1, end, '"');
            if (endOfName == end) {
                throwNotFound('"');
            }

            node->data = std::string(firstChar + 1, static_cast<size_t>(endOfName - (firstChar + 1)));

            auto it = endOfName + 1;
            skipWhitespace(it);

            if (it == end) {
                return it;
            }
            if (*it == '[') {
                node->left = new Node();
                node->right = new Node();
                it = parseExpression(it + 1, end, node->left);
                it = parseExpression(it, end, node->right);
                skipWhitespace(it);

                if (it == end) throwNotFound(']');
                if (*it == ']') {
                    return it + 1;
                }
                throwFoundChar(']', *it);
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

        struct Fact {
            string_view data;
            bool isTrue;

            Fact(const string_view &data, bool isTrue) : data(data), isTrue(isTrue) {}
        };

        bool findFacts(string_view name, Node *node, Vector<Fact> &facts) {
            if (node->left == nullptr && node->right == nullptr) {
                if (strcmp(name, node->data.c_str()) == 0) {
                    return true;
                } else {
                    return false;
                }
            }

            bool leftChild = findFacts(name, node->left, facts);
            bool rightChild = findFacts(name, node->right, facts);
            if (leftChild) {
                facts.emplace_back(node->data.c_str(), true);
            }
            if (rightChild) {
                facts.emplace_back(node->data.c_str(), false);
            }

            return leftChild || rightChild;
        }

        void addName(Node *curNode) const {
            char name[128];
            char diff[128];
            printf("Enter name of what you guessed:\n");
            scanf("%127s", name);
            printf("Enter fact which true for %s and false for %s?\n", name, curNode->data.c_str());
            scanf("%127s", diff);
            curNode->left = new Node();
            curNode->right = new Node();
            curNode->right->data = std::move(curNode->data);
            curNode->left->data = name;
            curNode->data = diff;
        }

        Node *findUserNode()  {
            Node *curNode = &root;
            while (true) {
                if (curNode->left == nullptr || curNode->right == nullptr) {
                    break;
                }
                printf("%s?\n", curNode->data.c_str());
                char ans[20] = "yes";
                scanf(" %19s", ans);
                if (ans[0] == 'y') {
                    curNode = curNode->left;
                } else if (ans[0] == 'n') {
                    curNode = curNode->right;
                }
            }
            return curNode;
        }

    public:
        DecisionTree(string_view data) : root() {
            parseExpression(data.begin(), data.end(), &root);
        }

        ~DecisionTree() = default;

        DecisionTree(const DecisionTree &) = delete;

        DecisionTree &operator=(const DecisionTree &) = delete;

        DecisionTree(DecisionTree &&other) = delete;

        DecisionTree &operator=(DecisionTree &&other) = delete;

        void serialize(FILE *fp) {
            serializeNode(fp, &root);
        }

        void guess() {
            printf("Let's guess...\n");
            Node *curNode = findUserNode();

            printf("Is it %s?\n", curNode->data.c_str());

            char ans[20] = "yes";
            scanf(" %19s", ans);
            if (ans[0] == 'y') {
                printf("EZ\n");
            } else if (ans[0] == 'n') {
                addName(curNode);
            }

            printf("Repeat?\n");
            scanf(" %19s", ans);
            if (ans[0] == 'y') {
                guess();
            }
        }

        void getDefenition(const char *name) {
            Vector<Fact> facts;
            findFacts(name, &root, facts);
            if (facts.empty()) {
                printf("I know nothing about %s (", name);
                return;
            }

            printf("%s ", name);
            for (size_t i = 0; i + 1 < facts.size(); i++) {
                if (facts[i].isTrue) {
                    printf("%s, ", facts[i].data.data());
                } else {
                    printf("not %s, ", facts[i].data.data());
                }
            }

            if (facts.back().isTrue) {
                printf("%s.", facts.back().data.data());
            } else {
                printf("not %s.", facts.back().data.data());
            }
        }
    };
}