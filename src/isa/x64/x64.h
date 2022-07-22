
#ifndef X_X64_H
#define X_X64_H

namespace x64 {

/**
 * Constants for x64 general purpose registers.
 */
enum Reg {
  rax,
  rcx,
  rdx,
  rbx,
  rsi,
  rdi,
  rsp,
  rbp,
  r8,
  r9,
  r10,
  r11,
  r12,
  r13,
  r14,
  r15
};

/**
 * Empty function: void func() {}
 */
struct EmptyFunction {
  unsigned char opc{0xC3}; // ret for x64

  void operator()() {
     auto func = reinterpret_cast<void (*)()>(this);
     func();
  }
};

} // namespace x64

#endif //X_X64_H
