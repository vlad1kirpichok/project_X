#include "utils.h"

#include <iostream>
#include <ios>

/**
 * Prints bytes of the specified data.
 *
 * @param data the pointer to data
 * @param size the size of data in bytes
 */
void print_bytes(const unsigned char* data, unsigned int size) {
  std::ios::fmtflags flags{std::cout.flags()};
  std::cout << std::hex;
  const unsigned char* current = data;
  for (unsigned int index = 0; index < size; ++index) {
    std::cout << static_cast<unsigned int>(*current);
    current++;
  }
  std::cout.flags(flags);
  std::cout << std::endl;
}
