
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
  rsp,
  rbp,
  rsi,
  rdi,
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
 * Encoding of Conditional Test (tttn) Field.
 */
enum Cond {
  O,            // 0000 = Overflow
  NO,           // 0001 = No overflow
  B, NAE = B,   // 0010 = Below, Not above or equal
  NB, AE = NB,  // 0011 = Not below, Above or equal
  E, Z = E,     // 0100 = Equal, Zero
  NE, NZ = NE,  // 0101 = Not equal, Not zero
  BE, NA = BE,  // 0110 = Below or equal, Not above
  NBE, A = NBE, // 0111 = Not below or equal, Above
  S,            // 1000 = Sign
  NS,           // 1001 = Not sign
  P, PE = P,    // 1010 = Parity, Parity Even
  NP, PO = NP,  // 1011 = Not parity, Parity Odd
  L, NGE = L,   // 1100 = Less than, Not greater than or equal to
  NL, GE = NL,  // 1101 = Not less than, Greater than or equal to
  LE, NG = LE,  // 1110 = Less than or equal to, Not greater than
  NLE, G = NLE, // 1111 = Not less
};

inline char offset_8bits(void* source, void* target) {
  return static_cast<char>(static_cast<char *>(target) - static_cast<char *>(source));
}

struct nop {
  unsigned char opc{0b10010000};
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

#pragma pack(1)
template <Reg REG, int IMM>
struct mov {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  unsigned int imm;

  mov()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b11000111}
    , reg{REG}
    , opc2{0b11000}
    , imm{IMM} {}
};

template <Reg REG, char IMM>
struct add {
  rex pref;
  unsigned char opc1;
  unsigned char reg : 3;
  unsigned char opc2 : 5;
  char imm;

  add()
    : pref{1, 0, 0, REG >> 3}
    , opc1{0b10000011}
    , reg{REG}
    , opc2{0b11000}
    , imm{IMM} {}
};

template <Reg REG1, Reg REG2>
struct cmp {
  rex pref;
  unsigned char opc1;
  unsigned char reg1 : 3;
  unsigned char reg2 : 3;
  unsigned char opc2 : 2;

  cmp()
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b00111001}
    , reg1{REG1}
    , reg2{REG2}
    , opc2{0b11} {}
};

struct jmp {
  unsigned char opc;
  char off;

  jmp(char offset)
    : opc{0b11101011}
    , off{offset} {}

  jmp(void* target)
    : jmp{static_cast<char>(offset_8bits(this, target) - sizeof(*this))} {}
};

template <Cond COND>
struct j {
  unsigned char opc : 4;
  unsigned char tttn : 4;
  char off;

  j(char offset)
    : opc{0b0111}
    , tttn{COND}
    , off{offset} {}

  j(void* target)
    : j{static_cast<char>(offset_8bits(this, target) - sizeof(*this))} {}
};

template <Reg REG1, Reg REG2, int DISP>
struct lea {
  rex pref;
  unsigned char opc1;
  unsigned char reg2 : 3;
  unsigned char reg1 : 3;
  unsigned char mod: 2;
  unsigned char disp;

  lea()
    : pref{1, 0, 0, REG1 >> 3}
    , opc1{0b10001101}
    , mod{0b01}
    , reg1{REG1}
    , reg2{REG2}
    , disp{DISP} {}
};

/**
 * Empty function: void func() {}
 */
#pragma pack(1)
struct EmptyFunction {
  mov<rax, 15> i13;
  add<rax, 7> i01;
  pushq<rax> i0;
  popq<rax> i1;
  ret i2;

  long operator()() {
     auto func = reinterpret_cast<long (*)()>(this);
     return func();
  }
};

} // namespace x64

#endif //X_X64_H
