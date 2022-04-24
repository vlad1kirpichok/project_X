
#ifndef X_RISCV64_H
#define X_RISCV64_H

#include <bitset>

namespace riscv64 {

/*
 * Operator for converting an instruction to a 32-bit value.
 */
#define I_TO_INT32 operator unsigned int() { return *reinterpret_cast<unsigned int *>(this); }

/**
* Constants for RISC-V registers.
*/
enum Reg {
    x0,
    zero = x0,
    x1,
    ra = x1
};

struct Rtype {
    I_TO_INT32
    unsigned int opcode : 7;
    unsigned int rd     : 5;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    unsigned int rs2    : 5;
    unsigned int funct7 : 7;
};

struct Itype  {
    I_TO_INT32
    unsigned int opcode : 7;
    unsigned int rd     : 5;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    int imm             : 12;
};

struct Utype  {
    I_TO_INT32
    unsigned int opcode : 7;
    unsigned int rd     : 5;
    unsigned int imm    : 20;
};

struct Stype  {
    I_TO_INT32
    unsigned int opcode : 7;
    unsigned int imm4_0 : 5;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    unsigned int rs2    : 5;
    int          imm11_5: 7;
    Stype(unsigned char opcode, unsigned char funct3, Reg rs1, Reg rs2, short imm)
        : opcode(opcode), funct3(funct3), rs1(rs1), rs2(rs2), imm4_0(imm), imm11_5(imm >> 5) {}
    int imm() {
        return (imm11_5 << 5) | imm4_0;
    }
};

struct Btype {
    I_TO_INT32
    unsigned int opcode : 7;
    unsigned int imm11  : 1;
    unsigned int imm4_1 : 4;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    unsigned int rs2    : 5;
    unsigned int imm10_5: 6;
    int imm12           : 1;
    Btype(unsigned char opcode, unsigned char funct3, Reg rs1, Reg rs2, short offset)
        : opcode(opcode), funct3(funct3), rs1(rs1), rs2(rs2),
          imm11(offset >> 11), imm4_1(offset >> 1), imm10_5(offset >> 5), imm12(offset >> 12) {
    }
    int imm() {
        return (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
    }
};

struct Jtype {
    I_TO_INT32
    unsigned int opcode  : 7;
    unsigned int rd      : 5;
    unsigned int imm19_12: 8;
    unsigned int imm11   : 1;
    unsigned int imm10_1 : 10;
    int imm20            : 1;
    Jtype(unsigned char opcode, Reg rd, int imm)
        : opcode(opcode), rd(rd), imm19_12(imm >> 12), imm11(imm >> 11), imm10_1(imm >> 1), imm20(imm >> 20) {}
    int imm() {
        return (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
    }
};

template <Reg RD, Reg RS1, Reg RS2>
struct add : Rtype {
    add() : Rtype{0b0110011, RD, 0b000, RS1, RS2, 0x0000000} { }
};

template <Reg RD, Reg RS1, short IMM>
struct addi : Itype {
    addi() : Itype{0b0010011, RD, 0b000, RS1, IMM} { }
};

template <Reg RD, unsigned int IMM>
struct lui : Utype {
    lui() : Utype{0b0110111, RD, IMM} {}
};

template <Reg RS2, short IMM, Reg RS1>
struct sw : Stype {
    sw() : Stype{0b0100011, 0b010, RS1, RS2, IMM} {}
};

template <Reg RS1, Reg RS2, short OFFSET>
struct beq : Btype {
    beq() : Btype{0b1100011, 0b000, RS1, RS2, OFFSET} {}
};

template <Reg RD, int OFFSET>
struct jal : Jtype {
    jal() : Jtype{0b1101111, RD, OFFSET} {}
};

} // namespace riscv64

#endif //X_RISCV64_H
