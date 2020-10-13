#include <iostream>
#include <fstream>
#include <regex>

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
    std::regex russian("[а-яА-Я]");
    for (const auto &str : lines) {
        if (std::regex_search(str.begin(), str.end(), russian)) {
            std::cout << str << std::endl;
         }
    }

    return 0;
}
