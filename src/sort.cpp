#include <stdexcept>
#include <iostream>

#include "onegin.h"

int main() {
    std::vector<int> a = {2, 7, 21, 1, 2, 2, 45, 5, 29, 1, 45, 100, 54};
    onegin::sort(a.begin(), a.end());


    return 0;
}
