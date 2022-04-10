
#include <iostream>

#include "inject/Inject.h"

using namespace xinject;

int main() {
  std::cout << "This is a test" << std::endl;
  int x = 0;
  inject("", x);
  return 0;
}
