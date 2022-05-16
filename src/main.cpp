
#include <iostream>

#include "inject/inject.h"

using namespace xinject;

int main(int argc, char* argv[]) {
  std::cout << "This is a test" << std::endl;
  if (argc < 2) {
    std::cout << "File to be patched is not specified!" << std::endl;
    return 1;
  }
  int x = 0;
  inject(argv[1], x);
  return 0;
}
