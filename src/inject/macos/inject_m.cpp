#include "inject_m.h"

#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <mach-o/loader.h>
#include <mach-o/loader.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace xinject {
namespace macos {

void map_file(const char *filename, void **content, size_t *size);

void inject(const char* app_path, const void* binary, size_t size) {
  std::cout << "MacOS: Injecting into '" << app_path << "' ..." << std::endl;
  void *file_content;
  size_t file_size;
  map_file(app_path, &file_content, &file_size);

  struct mach_header_64 *header;
  header = (struct mach_header_64 *)file_content;

  std::cout << std::hex << header->magic << std::endl;
  std::cout << std::hex << header->cputype << std::endl;
  std::cout << std::hex << header->filetype << std::endl;

  struct load_command * loader = (struct load_command *)(header + 1);
  for (uint32_t counter = 0; counter < header->ncmds; ++counter) {
    std::cout << std::hex << loader->cmd << std::endl;
    loader = (struct load_command *)(((char *)loader) + loader->cmdsize);
  }
}

void map_file(const char *filename, void **content, size_t *size) {
  struct stat info;
  int fd = open(filename, O_RDONLY);
  fstat(fd, &info);
  *size = info.st_size;
  *content = mmap(0, *size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (*content == MAP_FAILED) {
    std::cerr << "Error mapping file. Exiting...";
    exit(1);
  }
}

} // namespace macos
} // namespace xinject
