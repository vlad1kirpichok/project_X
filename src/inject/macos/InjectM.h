
#ifndef X_INJECTM_H
#define X_INJECTM_H

namespace xinject {
namespace macos {

template<typename T>
void inject(const std::string &app_path, const T &binary) {
  std::cout << "Hello from MacOS!" << std::endl;
}

} // namespace macos
} // namespace xinject

#endif //X_INJECTM_H
