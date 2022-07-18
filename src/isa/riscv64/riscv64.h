#ifndef X_RISCV64_H
#define X_RISCV64_H

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
        ra = x1,
        x2,
        sp = x2,
        x3,
        gp = x3,
        x4,
        tp = x4,
        x5,
        t0 = x5,
        x6,
        t1 = x6,
        x7,
        t2 = x7,
        x8,
        s0 = x8,
        fp = x8,
        x9,
        s1 = x9,
        x10,
        a0 = x10,
        x11,
        a1 = x11,
        x12,
        a2 = x12,
        x13,
        a3 = x13,
        x14,
        a4 = x14,
        x15,
        a5 = x15,
        x16,
        a6 = x16,
        x17,
        a7 = x17,
        x18,
        s2 = x18,
        x19,
        s3 = x19,
        x20,
        s4 = x20,
        x21,
        s5 = x21,
        x22,
        s6 = x22,
        x23,
        s7 = x23,
        x24,
        s8 = x24,
        x25,
        s9 = x25,
        x26,
        s10 = x26,
        x27,
        s11 = x27,
        x28,
        t3 = x28,
        x29,
        t4 = x29,
        x30,
        t5 = x30,
        x31,
        t6 = x31
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
                  imm11(offset >> 11), imm4_1(offset >> 1), imm10_5(offset >> 5), imm12(offset >> 12) { }
        int imm() {
            return (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
        }
        inline short offset(void* target) {
            void * source = static_cast<void *>(this);
            return static_cast<short>(static_cast<short *>(target) - static_cast<short *>(source));
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
        inline int offset(void* target) {
            void * source = static_cast<void *>(this);
            return static_cast<int>(static_cast<short *>(target) - static_cast<short *>(source));
        }
    };

    //R-type instructions RV32
    template <Reg RD, Reg RS1, Reg RS2>
    struct add : Rtype {
        add() : Rtype{0b0110011, RD, 0b000, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct sub : Rtype {
        sub() : Rtype{0b0110011, RD, 0b000, RS1, RS2, 0b0100000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct sll : Rtype {
        sll() : Rtype{0b0110011, RD, 0b001, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct slt : Rtype {
        slt() : Rtype{0b0110011, RD, 0b010, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct sltu : Rtype {
        sltu() : Rtype{0b0110011, RD, 0b011, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct xor_ : Rtype {
        xor_() : Rtype{0b0110011, RD, 0b100, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct srl : Rtype {
        srl() : Rtype{0b0110011, RD, 0b000, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct sra : Rtype {
        sra() : Rtype{0b0110011, RD, 0b101, RS1, RS2, 0b0100000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct or_ : Rtype {
        or_() : Rtype{0b0110011, RD, 0b110, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct and_ : Rtype {
        and_() : Rtype{0b0110011, RD, 0b111, RS1, RS2, 0b0000000} { }
    };

    //R-type instructions RV64
    template <Reg RD, Reg RS1, Reg RS2>
    struct addw : Rtype {
        addw() : Rtype{0b0111011, RD, 0b000, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct subw : Rtype {
        subw() : Rtype{0b0111011, RD, 0b000, RS1, RS2, 0b0100000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct sllw : Rtype {
        sllw() : Rtype{0b0111011, RD, 0b001, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct srlw : Rtype {
        srlw() : Rtype{0b0111011, RD, 0b101, RS1, RS2, 0b0000000} { }
    };

    template <Reg RD, Reg RS1, Reg RS2>
    struct sraw : Rtype {
        sraw() : Rtype{0b0111011, RD, 0b101, RS1, RS2, 0b0100000} { }
    };

    //I-type instructions RV32
    template <Reg RD, Reg RS1, short IMM>
    struct addi : Itype {
        addi() : Itype{0b0010011, RD, 0b000, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct slti : Itype {
        slti() : Itype{0b0010011, RD, 0b010, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct sltiu : Itype {
        sltiu() : Itype{0b0010011, RD, 0b011, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct xori : Itype {
        xori() : Itype{0b0010011, RD, 0b100, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct ori : Itype {
        ori() : Itype{0b0010011, RD, 0b110, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct andi : Itype {
        andi() : Itype{0b0010011, RD, 0b111, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct lb : Itype {
        lb() : Itype{0b0000011, RD, 0b000, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct lh : Itype {
        lh() : Itype{0b0000011, RD, 0b001, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct lw : Itype {
        lw() : Itype{0b0000011, RD, 0b010, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct lbu : Itype {
        lbu() : Itype{0b0000011, RD, 0b100, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct lhu : Itype {
        lhu() : Itype{0b0000011, RD, 0b101, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct jalr : Itype {
        jalr() : Itype{0b1100111, RD, 0b000, RS1, IMM} { }
    };

    //I-type instructions RV64
    template <Reg RD, Reg RS1, short IMM>
    struct lwu : Itype {
        lwu() : Itype{0b0000011, RD, 0b110, RS1, IMM} { }
    };

    template <Reg RD, short IMM, Reg RS1>
    struct ld : Itype {
        ld() : Itype{0b0000011, RD, 0b011, RS1, IMM} { }
    };

    template <Reg RD, Reg RS1, short IMM>
    struct addiw : Itype {
        addiw() : Itype{0b0011011, RD, 0b000, RS1, IMM} { }
    };

    //U-type instructions
    template <Reg RD, unsigned int IMM>
    struct lui : Utype {
        lui() : Utype{0b0110111, RD, IMM} {}
    };

    template <Reg RD, unsigned int IMM>
    struct auipc : Utype {
        auipc() : Utype{0b0010111, RD, IMM} {}
    };

    //S-type instructions RV32
    template <Reg RS2, short IMM, Reg RS1>
    struct sw : Stype {
        sw() : Stype{0b0100011, 0b010, RS1, RS2, IMM} {}
    };

    template <Reg RS2, short IMM, Reg RS1>
    struct sb : Stype {
        sb() : Stype{0b0100011, 0b000, RS1, RS2, IMM} {}
    };

    template <Reg RS2, short IMM, Reg RS1>
    struct sh : Stype {
        sh() : Stype{0b0100011, 0b001, RS1, RS2, IMM} {}
    };

    //S-type instructions RV64
    template <Reg RS2, short IMM, Reg RS1>
    struct sd : Stype {
        sd() : Stype{0b0100011, 0b011, RS1, RS2, IMM} {}
    };

    //B-type instructions
    template <Reg RS1, Reg RS2>
    struct beq : Btype {
        beq(short offset) : Btype{0b1100011, 0b000, RS1, RS2, offset} {}
        beq(void* target) : beq{offset(target)} {}
    };

    template <Reg RS1, Reg RS2, short OFFSET>
    struct bne : Btype {
        bne() : Btype{0b1100011, 0b001, RS1, RS2, OFFSET} {}
    };

    template <Reg RS1, Reg RS2, short OFFSET>
    struct blt : Btype {
        blt() : Btype{0b1100011, 0b100, RS1, RS2, OFFSET} {}
    };

    template <Reg RS1, Reg RS2, short OFFSET>
    struct bge : Btype {
        bge() : Btype{0b1100011, 0b101, RS1, RS2, OFFSET} {}
    };

    template <Reg RS1, Reg RS2, short OFFSET>
    struct bltu : Btype {
        bltu() : Btype{0b1100011, 0b110, RS1, RS2, OFFSET} {}
    };

    template <Reg RS1, Reg RS2, short OFFSET>
    struct bgeu : Btype {
        bgeu() : Btype{0b1100011, 0b111, RS1, RS2, OFFSET} {}
    };

    //J-type instructions
    template <Reg RD>
    struct jal : Jtype {
        jal(int offset) : Jtype{0b1101111, RD, offset} {}
        jal(void* target) : jal{offset(target)} {}
    };

#undef I_TO_INT32

    /**
     * Macro for saving to stack callee-saved registers (ra, s0-s3).
     */
    struct save_to_stack {
        addi<sp, sp, -40> i0;
        sd<ra, 0,  sp> i1;
        sd<s0, 8,  sp> i2;
        sd<s1, 16, sp> i3;
        sd<s2, 24, sp> i4;
        sd<s3, 32, sp> i5;
    };

    /**
     * Macro for loading from stack callee-saved registers (ra, s0-s3).
     */
    struct load_from_stack {
        ld<ra, 0,  sp> i0;
        ld<s0, 8,  sp> i1;
        ld<s1, 16, sp> i2;
        ld<s2, 24, sp> i3;
        ld<s3, 32, sp> i4;
        addi<sp, sp, 40> i5;
    };

    /**
    * Function to be injected.
    */
    template <unsigned char count>
    struct Prog {
        save_to_stack stack_save;
        add<s0, zero, zero> i1;
        addi<s1, zero, count> i2;
// loop start:
        beq<s0, s1> loop_start{&stack_load};
        addi<s0, s0, 1> i3;
        jal<zero> i4{&loop_start};
// loop end:
        load_from_stack stack_load;
        jalr<zero, ra, 0> ret;
    };

} // namespace riscv64

#endif //X_RISCV64_H
