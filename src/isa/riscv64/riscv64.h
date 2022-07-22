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
    struct slli : Itype {
        slli() : Itype{0b0010011, RD, 0b001, RS1, IMM} { }
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

    template <Reg RD, Reg RS1, short IMM = 0>
    struct ori : Itype {
        ori(short imm) : Itype{0b0010011, RD, 0b110, RS1, imm} { }
        ori() : ori{IMM} {}
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

    template <Reg RD, unsigned int IMM = 0>
    struct auipc : Utype {
        auipc(unsigned int imm) : Utype{0b0010111, RD, imm} {}
        auipc() : auipc{IMM} {}
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

    template <Reg RD, int imm>
    struct li32 {
        lui<RD, (imm >> 12)> i1; // imm<31..12>
        ori<RD, RD, static_cast<short>(imm) & 0x0FFF> i2; // imm<11..0>
    };

    template <Reg RD, long long imm>
    struct li64 {
        li32<RD, static_cast<int>(imm >> 32)> i1; // imm<63..32>
        slli<RD, RD, 11> i2;
        ori<RD, RD, static_cast<short>(imm >> 21) & 0x07FF> i3; // imm<31..21>
        slli<RD, RD, 11> i4;
        ori<RD, RD, static_cast<short>(imm >> 10) & 0x07FF> i5; // imm<20..10>
        slli<RD, RD, 10> i6;
        ori<RD, RD, static_cast<short>(imm) & 0x03FF> i7; // imm<9..0>
    };

    template <Reg RD>
    struct la {
        Utype i0;
        Itype i1;

        la(void * target) {
            long long delta = offset(target);
            long long hi = delta >> 32;
            if (hi != 0 && hi != -1L) {
                throw std::runtime_error("Out of range!");
            }
            i0 = auipc<RD>{delta >> 12};
            i1 = ori<RD, RD>{delta};
        }

        inline long long offset(void * target) {
            void * source = static_cast<void *>(this);
            return static_cast<long long>(static_cast<char *>(target) - static_cast<char *>(source));
        }
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
     * <p>
     * Calls the specified function in a loop and passes the specified data to it.
     *
     * @tparam data_size the data size
     * @tparam loop_count the loop count
     */
    template <unsigned int data_size, unsigned char loop_count>
    struct Function {
        typedef int (* func_t)(const char *);

        save_to_stack stack_save;
        add<s0, zero, zero> s0_count_zero;
        addi<s1, zero, loop_count> s1_max_count;
        la<s2> s2_data{data};
        la<s3> s3_func_addr{reinterpret_cast<void *>(function)};
        ld<s3, 0, s3> s3_func;
// loop start:
        beq<s0, s1> loop_start{&stack_load};
        add<a0, zero, s2> init_func_arg;
        jalr<ra, s3, 0> call_func;
        addi<s0, s0, 1> inc_counter;
        jal<zero> repeat_loop{&loop_start};
// loop end:
        load_from_stack stack_load;
        jalr<zero, ra, 0> ret;
// Data for the function to be called. Placed after executable code.
        char data[data_size] {0};
// Address of the function to be called.
        func_t function;

        /**
         * Constructor.
         *
         * @param data data for the function
         */
        explicit Function(func_t func, const char * data)
            : function{func} {
            const char * src = data;
            char * dst = this->data;
            for (unsigned int i = 0; i < data_size; ++i) {
                *dst++ = *src++;
            }
        }

        /**
         * Calls the function described by this data structure.
         *
         * Requirement: works correctly only on a RISC-V computer.
         */
        void operator()() {
            auto func = reinterpret_cast<void (*)()>(this);
            func();
        }
    };

} // namespace riscv64

#endif //X_RISCV64_H
