
#ifndef X_INJECT_H
#define X_INJECT_H

#include <string>

#ifdef __APPLE__
  #include "macos/inject_m.h"
#elif _WIN32
  #include "windows/inject_w.h"
#elif __linux__
  #include "linux/inject_l.h"
#endif

/**
 * Provides functions for saving, loading and injecting binary code into executables.
 */
namespace xinject {

/**
 * Injects the specified binary code in the specified executable file.
 *
 * @tparam T the data type for binary code
 * @param path the executable file path
 * @param binary the binary code
 */
template<typename T>
void inject(const std::string &path, const T &binary) {
#ifdef __APPLE__
  macos::inject(path, binary);
#elif _WIN32
  windows::inject(path, binary);
#elif __linux__
  linux::inject(path, binary);
#else
  #error "Unknown OS!"
#endif
}

/**
 * Saves the specified binary data to the specified file.
 *
 * @param path the file path
 * @param data the binary data
 * @param size the binary data size
 */
void save(const std::string &path, const char* data, size_t size);

/**
 * Saves the specified data of the specified type to a binary file.
 *
 * @tparam T the data type
 * @param path the file path
 * @param binary the data
 */
template<typename T>
void save(const std::string &path, const T &binary) {
  save(path, reinterpret_cast<const char*>(&binary), sizeof(binary));
}

/**
 * Loads binary data from the specified file.
 *
 * @param path the file path
 * @param data the buffer for storing binary data
 * @param size the binary data size
 */
void load(const std::string &path, char* data, size_t size);

/**
 * Loads data of the specified type from the specified file.
 *
 * @tparam T the data type
 * @param path the file path
 * @return the loaded data
 */
template<typename T>
T load(const std::string &path) {
  T data;
  load(path, reinterpret_cast<char *>(&data), sizeof(data));
  return data;
}

} // namespace xinject

#endif // PROJECT_X_INJECT_H
