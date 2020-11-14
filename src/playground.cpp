#include "list.hpp"
#include <string>
#include <utility>
#include <iostream>
#include <numeric>

int main() {
    mipt::List<int> list;
    list.emplace_back(1);
    list.emplace_back(22);
    list.emplace_back(-20);
    list.emplace_back(50);
    list.emplace_back(999999);
    list.emplace_back(0);
    std::cout << std::reduce(list.begin(), list.end(), ) << "\n";
    for (auto item : list) {
        std::cout << item << std::endl;
    }
}