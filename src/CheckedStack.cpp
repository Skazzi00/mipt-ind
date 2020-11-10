#include <cstdio>

#include "stack.hpp"
#include <utility>
#include "list.hpp"
int cnt=  0;
struct NonCopyable {
    int a;
    NonCopyable() {}
    NonCopyable(const NonCopyable &) = default;
    NonCopyable(NonCopyable &&) = delete;
};

int main() {
    using namespace mipt;
    List<int>::Node a;
    List<int>::Node b;
    b.linkPrev(a);

}