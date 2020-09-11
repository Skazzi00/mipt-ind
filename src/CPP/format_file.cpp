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
        if (!line.empty())
            lines.push_back(line);
    }

    return 0;
}
