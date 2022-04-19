
#ifndef X_INJECT_H
#define X_INJECT_H

#include <iostream>
#include <string>

namespace xinject {

template<typename T>
void inject(const std::string &app_path, const T &binary) {
#ifdef __APPLE__
  std::cout << "Hello from MacOS!" << std::endl;
#elif _WIN32
  std::cout << "Hello from Windows OS!" <<     std::endl;
#elif __linux__
  std::cout << "Hello from Linux OS!" <<     std::endl;
#endif
}

} // namespace xinject

#endif // PROJECT_X_INJECT_H
