
#include <iostream>

#include "inject/inject.h"
#include "isa/aarch64/aarch64.h"
#include "isa/riscv64/riscv64.h"
#include "isa/x64/x64.h"

using namespace xinject;

namespace aarch64 {
/**
 * Aarch64 program to injected.
 */
struct Program {
  mov<x1, x2> i1;
  mov<w1, w4> i2;
  ret<> i3;
};

void test() {
    std::cout << std::hex << mov<x1, x2>{} << std::endl;
    std::cout << std::hex << mov<x1, x2>{} << std::endl;
    std::cout << std::hex << ret<>{} << std::endl;
}

}

namespace riscv64 {
/**
 * RISC-V program to injected.
 */
struct Program {
  addi<t0, zero, 1> i1;
  add<t1, zero, t0> i2;
};
}

namespace x64 {
/**
 * x64 program to injected.
 */
struct Program {
  // TODO
};
}

int main(int argc, char* argv[]) {
  aarch64::test();
  xinject::save("aarch64_test.bin", aarch64::Program{});
  xinject::save("riscv64_test.bin", riscv64::Program{});
  xinject::save("x64_test.bin",     x64::Program{});

  std::cout << "This is a test" << std::endl;
  if (argc < 2) {
    std::cout << "File to be patched is not specified!" << std::endl;
    return 1;
  }
  int x = 0;
  xinject::inject(argv[1], x);

  return 0;
}
