
#ifndef X_INJECT_M_H
#define X_INJECT_M_H

#include <string>

namespace xinject {
namespace macos {

void inject(const char* app_path, const void* binary, size_t size);

template<typename T>
void inject(const std::string &app_path, const T &binary) {
  inject(app_path.c_str(), &binary, sizeof(binary));
}

} // namespace macos
} // namespace xinject

#endif //X_INJECT_M_H
