#include <cstdio>
#include <optional>

struct mytype {
  int x;
  int y;
  int z;

  void method() {

  }
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
  optional() : isExist(false) {

  }

  T *operator->() { return &val; }
};

int main() {
  optional<mytype> opt;

  opt->method();
  printf("%lu\n", sizeof(opt));

}