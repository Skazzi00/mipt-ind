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
                errorMessage.append("Error occurred while parsing: Expected '");
                errorMessage.push_back(expected);
                errorMessage.append("', but found '");
                errorMessage.push_back(found);
                errorMessage.push_back('\'');
                throw parse_error(std::move(errorMessage));
            };

            auto firstChar = input;
            skipWhitespace(firstChar);

            if (firstChar == end) {
                throw parse_error("Error occurred while parsing: Expected '\"', but not found");
            }

            if (*firstChar != '"') {
                throwFoundChar('"', *firstChar);
            }

            auto endOfName = std::find(firstChar + 1, end, '"');
            if (endOfName == end) {
                throw parse_error("Error occurred while parsing: Expected '\"', but not found");
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
                if (it == end) return it;
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

    public:
        DecisionTree(string_view data) {
            parseExpression(data.begin(), data.end(), &root);
        }

        void serialize(FILE *fp) {
            serializeNode(fp, &root);
        }

        void guess() {
            Node *curNode = &root;
            printf("Let's guess...\n");
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
            printf("Is it %s?\n", curNode->data.c_str());
            char ans[20] = "yes";
            scanf(" %19s", ans);

            if (ans[0] == 'y') {
                printf("EZ\n");
            } else if (ans[0] == 'n') {
                char name[128];
                char diff[128];
                printf("Enter name of what you guessed:\n");
                scanf("%127s", name);
                printf("What difference beetween %s and %s?\n", curNode->data.c_str(), name);
                scanf("%127s", diff);
                curNode->left = new Node();
                curNode->right = new Node();
                curNode->right->data = std::move(curNode->data);
                curNode->left->data = name;
                curNode->data = diff;
            }

            printf("Repeat?\n");
            scanf(" %19s", ans);
            if (ans[0] == 'y') {
                guess();
            }
        }
    };
}