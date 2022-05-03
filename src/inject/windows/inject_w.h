
#ifndef X_INJECTW_H
#define X_INJECTW_H

#include <iostream>

namespace xinject {
namespace windows {

template<typename T>
void inject(const std::string &app_path, const T &binary) {
  std::cout << "Hello from Windows OS!" << std::endl;
}

} // namespace windows
} // namespace xinject

#endif //X_INJECTW_H
