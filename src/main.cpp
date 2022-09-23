
#include <iostream>

#include "inject/inject.h"
#include "isa/utils.h"
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
  b_<eq, 4>   i3;
  bl<4>       i4;
  ret<>       i5;
};

void test() {
  std::cout << std::hex << mov<x1, x2>{} << std::endl;
  std::cout << std::hex << mov<x1, x2>{} << std::endl;
  std::cout << std::hex << bl<4>{} << std::endl;
}

} // namespace aarch64

namespace riscv64 {
/**
 * RISC-V program to injected.
 */
struct Program {
  addi<t0, zero, 1> i1;
  add<t1, zero, t0> i2;
};
} // namespace riscv64

namespace x64 {
/**
 * x64 program to injected.
 */
struct Program {
  pushq<rax> i0;
  inc<rax> i01;
  dec<rax> i02;
  popq<rax> i1;
  ret i2;
};

void test() {
  std::cout << "----------------" << std::endl;
  print_bytes(pushq<rax>{});
  print_bytes(inc<rax>{});
  print_bytes(dec<rax>{});
  print_bytes(popq<rax>{});
  print_bytes(mov<rax, 3>{});
  print_bytes(cmp<rcx, rdx>{});
  print_bytes(jmp{char{2}});
  print_bytes(j<G>{char{2}});
  std::cout << "/------------------" << std::endl;
  print_bytes(lea<rdi, rax, 0x78>{});
  std::cout << "/------------------" << std::endl;
  std::cout << sizeof(mov<rax, 3>{}) << std::endl;
  std::cout << "------------------" << std::endl;
  print_bytes(add<rdx, 1>{});
}
} // namespace x64

int main(int argc, char* argv[]) {
  x64::test();
  long res  = x64::EmptyFunction()();
  std::cout << "RES = " << res << std::endl;
  std::cout << "SIZE = " << sizeof(x64::EmptyFunction) << std::endl;

  aarch64::test();
  xinject::save("aarch64_test.bin", aarch64::Program{});
  xinject::save("riscv64_test.bin", riscv64::Program{});
  xinject::save("x64_test.bin",     x64::Program{});

  riscv64::Function<8, 10> proga{puts, "hello"};
  std::cout << sizeof(proga) << std::endl;
  std::cout << "This is a test" << std::endl;
  if (argc < 2) {
    std::cout << "File to be patched is not specified!" << std::endl;
    return 1;
  }
  int x = 0;
  xinject::inject(argv[1], x);
  return 0;
}
