#include <iostream>
#include <fstream>

#include "CPP/onegin.h"

int main(int argc, const char **argv) {
    std::ifstream fileInput;
    if (argc == 2) {
        fileInput.open(argv[1]);
    }
    std::istream &in = fileInput.is_open() ? fileInput : std::cin;
    std::vector<std::string> lines;
    std::string line;
    while (getline(in, line)) {
        lines.push_back(line);
    }
    onegin::sort(lines.begin(), lines.end());
    for (const auto &i : lines) {
        std::cout << i << std::endl;
    }
    return 0;
}
