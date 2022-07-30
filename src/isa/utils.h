
#ifndef PROJECT_X_UTILS_H
#define PROJECT_X_UTILS_H

/**
 * Prints bytes of the specified data (in hex).
 *
 * @param data the pointer to data
 * @param size the size of data in bytes
 */
void print_bytes(const unsigned char* data, unsigned int size);

/**
 * Prints bytes of the specified data (in hex).
 *
 * @tparam T the data type
 * @param data the data
 */
template <typename T>
void print_bytes(const T& data) {
  print_bytes(reinterpret_cast<const unsigned char*>(&data), sizeof(data));
}

#endif //PROJECT_X_UTILS_H
