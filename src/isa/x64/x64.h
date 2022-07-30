
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
  unsigned char reg : 3;
  unsigned char opc : 5;

  pushq()
      : reg{REG}
      , opc{0b01010} {
      static_assert(REG <= 0b0111, "1-byte encoding supports only rax-rbp");
  }
};

template <Reg REG>
struct popq {
  unsigned char reg : 3;
  unsigned char opc : 5;

  popq()
    : reg{REG}
    , opc{0b01011} {
    static_assert(REG <= 0b0111, "1-byte encoding supports only rax-rbp");
  }
};

struct rex {
  unsigned char b : 1;
  unsigned char x : 1;
  unsigned char r : 1;
  unsigned char w : 1;
  unsigned char pref : 4;

  rex(unsigned char w, unsigned char r, unsigned char x, unsigned char b)
    : b{b}
    , x{x}
    , r{r}
    , w{w}
    , pref{0b0100} {}
};

template <Reg REG>
struct inc {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;

  inc()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11111111}
    , reg{REG}
    , opc2{0b11000} {}
};

template <Reg REG>
struct dec {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;

  dec()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11111111}
    , reg{REG}
    , opc2{0b11001} {}
};

/**
 * Empty function: void func() {}
 */
struct EmptyFunction {
  pushq<rax> i0;
  inc<rax> i01;
  dec<rax> i02;
  popq<rax> i1;
  ret i2;

  void operator()() {
     auto func = reinterpret_cast<void (*)()>(this);
     func();
  }
};

} // namespace x64

#endif //X_X64_H
