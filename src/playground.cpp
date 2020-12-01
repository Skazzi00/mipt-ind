#include <cstdio>
#include <optional>

struct mytype {
  int x;
  int y;
  int z;
  mytype() {
    printf("dummy ctor called\n");
  }
};

template<typename T>
class optional {
  union {
    T val;
  };
  bool isExist;
 public:
  optional() : isExist(false) {}
};

int main() {
  std::optional<mytype> opt;

  printf("%lu\n", sizeof(opt));

}