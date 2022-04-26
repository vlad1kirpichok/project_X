#include "inject_m.h"

#include <iostream>

namespace xinject {
namespace macos {

void inject(const char* app_path, const void* binary, size_t size) {
  std::cout << "Hello from MacOS!" << std::endl;
}

} // namespace macos
} // namespace xinject
