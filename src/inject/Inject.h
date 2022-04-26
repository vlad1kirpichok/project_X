
#ifndef X_INJECT_H
#define X_INJECT_H

#include <iostream>
#include <string>

#ifdef __APPLE__
  #include "macos/InjectM.h"
#elif _WIN32
  #include "windows/InjectW.h"
#elif __linux__
  #include "linux/InjectL.h"
#endif

namespace xinject {

template<typename T>
void inject(const std::string &app_path, const T &binary) {
#ifdef __APPLE__
  macos::inject(app_path, binary);
#elif _WIN32
  windows::inject(app_path, binary);
#elif __linux__
  linux::inject(app_path, binary);
#else
  std::cerr << "Unknown OS!" << std::endl;
#endif
}

} // namespace xinject

#endif // PROJECT_X_INJECT_H
