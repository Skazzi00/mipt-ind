#include <iostream>
#include <fstream>

#include "CPP/onegin.h"

int main(int argc, const char **argv) {
    std::ifstream fileInput;
    if (argc == 2) {
        fileInput.open(argv[1]);
    }
    std::istream &in = fileInput.is_open() ? fileInput : std::cin;
    std::string data((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());
    std::size_t linesCnt = static_cast<size_t>(std::count(data.begin(), data.end(), '\n') + 1);
    std::vector<std::string_view> lines(linesCnt);
    std::size_t curLine = 0;
    std::size_t cur = 0;
    for (std::size_t i = 0; i < data.length(); ++i) {
        if(data[i] == '\n') {
            lines[curLine++] = std::string_view(data.data() + cur, i - cur);
            cur = i + 1;
        }
    }
    lines.back() = std::string_view(data.data() + cur, data.length() - cur);
    std::sort(lines.begin(), lines.end());
    for (const auto &i : lines) {
        std::cout << i << std::endl;
    }
    return 0;
}
