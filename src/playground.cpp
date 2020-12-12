#include <iostream>
#include <cstdio>
#include <optional>
#include <vector>

class G {
  const char *s;

  struct BinOp {
    char str;
    int (*func)(int, int);
    BinOp(char str, int (*func)(int, int)) : str(str), func(func) {}
  };

  std::vector<BinOp> addRules;
  std::vector<BinOp> mulRules;

  static void error() {
    exit(1);
  }

  bool trySkip(char ch) {
    if (*s == ch) {
      s++;
      return true;
    }
    return false;
  }

  void require(char ch) {
    if (*s != ch) {
      //print expected $ch found $s
      error();
    }
    s++;
  }

  int getE() {
    int val = getT();
    for (size_t i = 0; i < addRules.size(); i++) {
      const BinOp &item = addRules[i];
      if (item.str != *s) continue;
      s++;
      int val2 = getT();
      val = item.func(val, val2);
      i = 0;
    }
    return val;
  }

  int getT() {
    int val = getP();
    for (size_t i = 0; i < mulRules.size(); i++) {
      const BinOp &item = mulRules[i];
      if (item.str != *s) continue;
      s++;
      int val2 = getP();
      val = item.func(val, val2);
      i = 0;
    }
    return val;
  }

  int getP() {
    int val = 0;
    if (*s == '(') {
      require('(');
      val = getE();
      require(')');
    } else {
      val = getN();
    }
    return val;
  }
  int getN() {
    int val = 0;
    do {
      if (!isdigit(*s)) error();
      val = val * 10 + (*s - '0');
      s++;
    } while (isdigit(*s));
    return val;
  }

 public:
  explicit G() {}

  G &addAdditionRule(char op, int(*func)(int, int)) {
    addRules.emplace_back(op, func);
    return *this;
  }

  G &addMulRule(char op, int(*func)(int, int)) {
    mulRules.emplace_back(op, func);
    return *this;
  }

  int parse(const char *string) {
    s = string;
    int val = getE();
    require('\0');
    return val;
  }

};

int main() {
  G parser;
  parser.addAdditionRule('+', [](int a, int b) { return a + b; })
      .addAdditionRule('-', [](int a, int b) { return a - b; })
      .addMulRule('*', [](int a, int b) { return a * b; })
      .addMulRule('/', [](int a, int b) { return a / b; })
      .addAdditionRule('^', [](int a, int b) {
        printf("xor happened\n");
        return a ^ b;
      });

  printf("%d", parser.parse("2^3^1^100*22"));
  return 0;
}