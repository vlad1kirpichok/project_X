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
        w31,
        wzr = w31,
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
        fp = x29,
        x30,
        lr = x30,
        x31,
        xzr = x31,
        sp = x31
    };


/**
 * Condition code for branch instructions.
 */
    enum Cond {
        eq  = 0b0000,
        neq = 0b0001,
        hi  = 0b1000,
        hs  = 0b0010,
        ls  = 0b1001,
        lo  = 0b0011,
        gt  = 0b1100,
        ge  = 0b1010,
        le  = 0b1101,
        lt  = 0b1011,
        cs  = 0b0010,
        cc  = 0b0011,
        vs  = 0b0110,
        vc  = 0b0111,
        mi  = 0b0100,
        pl  = 0b0101,
        al  = 0b1110,
        nv  = 0b1111
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

    template <Reg RD, Reg RN, int IMM>
    struct add {
        I_TO_INT32

        unsigned int rd   : 5;
        unsigned int rn   : 5;
        unsigned int imm12 : 12;
        unsigned int sh   : 1;
        unsigned int opc  : 8;
        unsigned int sf   : 1;

        add()
                : rd{RD}, rn{RN}, imm12{IMM}, sh{0}, opc{0b010001001}, sf{RD >> 5} { }

        static_assert((RD >> 5) == (RN >> 5), "All registers must be either 32 or 64 bit!");
    };

    template <Reg RT, Reg RT2, Reg RN, int IMM>
    struct stp {
        I_TO_INT32

        unsigned int rt   : 5;
        unsigned int rt2  : 5;
        unsigned int rn   : 5;
        unsigned int imm7 : 7;
        unsigned int opc  : 10;

        stp()
                : rt{RT}, rn{RN}, rt2{RT2}, imm7{IMM}, opc{0b0100010101} {}
    };

    template <Reg RN, int IMM>
    struct cmp {
        I_TO_INT32

        unsigned int rd   : 5;
        unsigned int rn   : 5;
        unsigned int imm12   : 12;
        unsigned int sh   : 1;
        unsigned int opc  : 8;
        unsigned int sf   : 1;

        cmp()
                : rd{0b11111}, rn{RN}, imm12{IMM}, sh{0}, opc{0b01000111}, sf{RN >> 5} {}
    };

    template <Reg RT, Reg RT2, Reg RN, int IMM>
    struct ldp {
        I_TO_INT32

        unsigned int rt   : 5;
        unsigned int rn   : 5;
        unsigned int rt2  : 5;
        unsigned int imm7 : 7;
        unsigned int opc  : 9;
        unsigned int sp   : 1;

        ldp()
                : rt{RT}, rn{RN}, rt2{RT2}, imm7{IMM}, opc{0b111001010}, sp{RT >> 5} {}

        static_assert((RT >> 5) == (RT2 >> 5) == (RN >> 5), "All registers must be either 32 or 64 bit!");
    };

    template <Reg RD, Reg RN, int IMM>
    struct subs {
        I_TO_INT32

        unsigned int rd   : 5;
        unsigned int rn   : 5;
        unsigned int imm12 : 12;
        unsigned int sh   : 1;
        unsigned int opc  : 8;
        unsigned int sp   : 1;

        subs()
                : rd{RD}, rn{RN}, imm12{IMM}, sh{0}, opc{0b01000111}, sp{RD >> 5} {}
        static_assert((RD >> 5) == (RN >> 5), "All registers must be either 32 or 64 bit!");
    };

    template <int OFFSET>
    struct b {
        I_TO_INT32

        int imm26 : 26;
        unsigned int opc : 6;

        b()
                : imm26{OFFSET >> 2}, opc{0b000101} {}
    };

    template <Cond C, int OFFSET>
    struct b_ {
        I_TO_INT32

        unsigned int cond  : 4;
        unsigned int n     : 1;
        unsigned int imm19 : 19;
        unsigned int opc   : 8;

        b_()
                : cond{C}, n{0}, imm19{OFFSET >> 2}, opc{0b01010100} { }
    };

    template <int OFFSET>
    struct bl {
        I_TO_INT32

        int imm26 : 26;
        unsigned int opc : 6;

        bl()
                : imm26{OFFSET >> 2}, opc{0b100101} {}
    };

#undef I_TO_INT32

/**
 * Empty function: void func() {}
 */
    struct EmptyFunction {
        ret<> i0{};

        void operator()() {
            auto func = reinterpret_cast<void (*)()>(this);
            func();
        }
    };

} // namespace aarch64

#endif //X_AARCH64_H
