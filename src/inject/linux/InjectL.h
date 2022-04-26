
#ifndef X_INJECTL_H
#define X_INJECTL_H

namespace xinject {
namespace linux {

template<typename T>
void inject(const std::string &app_path, const T &binary) {
  std::cout << "Hello from Linux OS!" << std::endl;
}

} // namespace linux
} // namespace xinject

#endif //X_INJECTL_H
