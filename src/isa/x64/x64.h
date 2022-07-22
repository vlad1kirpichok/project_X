
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

struct ret {
  unsigned char opc{0xC3};
};

template <Reg REG>
struct pushq {
  unsigned char opc : 5;
  unsigned char reg : 3;

  pushq()
      : opc{(unsigned char) 0x01010}, reg{REG} {
      static_assert(REG <= 0b0111, "1-byte encoding supports only rax-rbp");
  }
};

template <Reg REG>
struct popq {
    unsigned char opc : 5;
    unsigned char reg : 3;

    popq()
        : opc{(unsigned char) 0x01011}, reg{REG} {
        static_assert(REG <= 0b0111, "1-byte encoding supports only rax-rbp");
    }
};

/**
 * Empty function: void func() {}
 */
struct EmptyFunction {
  pushq<rax> i0;
  popq<rax> i1;
  ret i2;

  void operator()() {
     auto func = reinterpret_cast<void (*)()>(this);
     func();
  }
};

} // namespace x64

#endif //X_X64_H
