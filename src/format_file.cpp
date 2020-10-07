#include <iostream>
#include <fstream>

#include "onegin.hpp"

int main(int argc, const char **argv) {
    std::ifstream fileInput;
    if (argc == 2) {
        fileInput.open(argv[1]);
    }
    std::istream &in = fileInput.is_open() ? fileInput : std::cin;
    std::vector<std::string> lines;
    std::string line;
    while (getline(in, line)) {
        if (!line.empty())
            lines.emplace_back(std::move(line));
    }
    for (const auto &str : lines) {
        bool flag = false;
        for (const auto ch : str) {
            if (("а" <= std::string(1, ch) && std::string(1, ch) <= "я") ) {
                flag = true;
                break;
            }
        }
        if (flag) {
            std::cout << str << std::endl;
         }
    }

    return 0;
}
