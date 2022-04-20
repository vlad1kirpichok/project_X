
#ifndef X_RISCV64_H
#define X_RISCV64_H

namespace riscv64 {

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
    unsigned int opcode : 7;
    unsigned int rd     : 5;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    unsigned int rs2    : 5;
    unsigned int funct7 : 7;
    operator unsigned int() {  return *reinterpret_cast<unsigned int *>(this); }
};

struct Itype  {
    unsigned int opcode : 7;
    unsigned int rd     : 5;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    unsigned int imm    : 12;
    operator unsigned int() {  return *reinterpret_cast<unsigned int *>(this); }
};

struct Utype  {
    unsigned int opcode : 7;
    unsigned int rd     : 5;
    unsigned int imm    : 20;
    operator unsigned int() {  return *reinterpret_cast<unsigned int *>(this); }
};

struct Stype  {
    unsigned int opcode : 7;
    unsigned int imm4_0 : 5;
    unsigned int funct3 : 3;
    unsigned int rs1    : 5;
    unsigned int rs2    : 5;
    unsigned int imm11_5: 7;
    operator unsigned int() {  return *reinterpret_cast<unsigned int *>(this); }
};

struct Btype {
    operator unsigned int() {  return *reinterpret_cast<unsigned int *>(this); }

    // imm[12|10:5] rs2 rs1 funct3 imm[4:1|11] opcode
};

struct Jtype {

};

template <Reg rd, Reg rs1, Reg rs2>
struct add : Rtype {
    add() : Rtype{0b0110011, rd, 0b000, rs1, rs2, 0x0000000} { }
};

template <Reg rd, Reg rs1, unsigned int imm>
struct addi : Itype {
    addi() : Itype{0b0010011, rd, 0b000, rs1, imm} { }
};

template <Reg rd, unsigned int imm>
struct lui : Utype {
    lui() : Utype{0b0110111, rd, imm} {}
};

template <Reg rs2, int offset, Reg rs1>
    struct sw : Stype {
        sw() : Stype{0b0100011, offset, 0b010, rs1, rs2, offset >> 5} {}
    };

} // namespace riscv64

#endif //X_RISCV64_H
