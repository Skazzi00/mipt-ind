#include <cstdio>

#include "stack.hpp"



int main() {
    using namespace mipt;
    CheckedStack<int> stack;
    stack.push(123);
    stack.push(367);
    *((char*)&stack + 8)  = 14;
    printf("%d", stack.top());
}