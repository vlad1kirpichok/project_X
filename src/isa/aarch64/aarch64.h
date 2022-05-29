
#ifndef X_AARCH64_H
#define X_AARCH64_H

namespace aarch64 {

/**
 * Constants for 64-bit ARM registers.
 */
enum Reg {
  // 32-bit registers: have numbers 0..31 encoded by 5 bits
  w0,
  w1,
  w2,
  w3,
  w4,
  w5,
  w6,
  w7,
  w8,
  w9,
  w10,
  w11,
  w12,
  w13,
  w14,
  w15,
  w16,
  w17,
  w18,
  w19,
  w20,
  w21,
  w22,
  w23,
  w24,
  w25,
  w26,
  w27,
  w28,
  w29,
  w30,
  wsp, // if required to use only 32-bit registers, use this instead of "sp".
  // 64-bit registers: have numbers 0..31 encoded by 5 bits
  // the 6th bit that is 1 is used instruction opcode to state it deals with 64-bit registers.
  x0,
  x1,
  x2,
  x3,
  x4,
  x5,
  x6,
  x7,
  x8,
  x9,
  x10,
  x11,
  x12,
  x13,
  x14,
  x15,
  x16,
  x17,
  x18,
  x19,
  x20,
  x21,
  x22,
  x23,
  x24,
  x25,
  x26,
  x27,
  x28,
  x29,
  x30,
  sp
};

/*
 * Operator for converting an instruction to a 32-bit value.
 */
#define I_TO_INT32 operator unsigned int() { return *reinterpret_cast<unsigned int *>(this); }

template <Reg RD, Reg RM>
struct mov {
  I_TO_INT32

  unsigned int rd   : 5;
  unsigned int rn   : 5;
  unsigned int imm6 : 6;
  unsigned int rm   : 5;
  unsigned int n    : 1;
  unsigned int shift: 2;
  unsigned int opc  : 7;
  unsigned int sf   : 1;

  mov()
    : rd{RD}, rn{0b11111}, imm6{0}, rm{RM}, n{0}, shift{0}, opc{0b0101010}, sf{RD >> 5} {
    static_assert((RD >> 5) == (RM >> 5), "All registers must be either 32 or 64 bit!");
  }
};

template <Reg RN = x30>
struct ret {
  I_TO_INT32

  unsigned int rm   : 5;
  unsigned int rn   : 5;
  unsigned int opc  : 22;

  ret()
    : rm{0}, rn{RN}, opc{0b1101011001011111000000} { }
};

#undef I_TO_INT32

} // namespace aarch64

#endif //X_AARCH64_H
