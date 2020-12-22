#include <cstdio>

#include "string_pool.hpp"
#include "decision_tree.hpp"
extern "C" {
#include "fileutil.h"
}

int main() {
  using namespace mipt;

  FILE *db = fopen("akinator.db", "r");
  size_t dbStrSize = static_cast<size_t>(getFileSize(db));
  char *dbStr = static_cast<char *>(malloc(dbStrSize + 1));
  fread(dbStr, sizeof(char), static_cast<size_t>(dbStrSize), db);
  dbStr[dbStrSize] = '\0';
  fclose(db);

  StringPool::Allocator allocator;
  DecisionTree decision_tree(dbStr, &allocator);

  while (true) {
    printf("Choose mode:\n"
           "1 - guess game\n"
           "2 - get definition of object\n"
           "3 - get difference\n"
           "4 - dump\n"
           "5 - quit\n");
    int mode;
    scanf("%d", &mode);
    bool quit = false;
    switch (mode) {
      case 1: decision_tree.guess();
        break;
      case 2: decision_tree.enterNameAndGetDef();
        break;
      case 3: decision_tree.enterNamesAndGetDiff();
        break;
      case 4: decision_tree.dump();
        break;
      case 5: quit = true;
        break;
    }
    if (quit) break;
  }

  FILE *dbOut = fopen("akinator.db", "w");
  decision_tree.serialize(dbOut);
  fclose(dbOut);
}