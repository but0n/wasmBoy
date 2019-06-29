// #include "cpu.h"
#include <stdio.h>
#define WASM_EXPORT __attribute__((visibility("default")))

static void sub_A_B();
static void add_A_B();


#define REG_AMOUNT 6

static unsigned short cache[REG_AMOUNT] = {};

// 1 BYTE REG
#define ADDR_F  0
#define ADDR_A  1
#define ADDR_B  3
#define ADDR_C  2
#define ADDR_D  5
#define ADDR_E  4
#define ADDR_H  7
#define ADDR_L  6

// 2 BYTE REG
#define ADDR_BC 1
#define ADDR_DE 2
#define ADDR_HL 3
#define ADDR_SP 4
#define ADDR_PC 5

// 8-bit registers
#define F   ((unsigned char *)cache)[ADDR_F]
#define A   ((unsigned char *)cache)[ADDR_A]
#define B   ((unsigned char *)cache)[ADDR_B]
#define C   ((unsigned char *)cache)[ADDR_C]
#define D   ((unsigned char *)cache)[ADDR_D]
#define E   ((unsigned char *)cache)[ADDR_E]
#define H   ((unsigned char *)cache)[ADDR_H]
#define L   ((unsigned char *)cache)[ADDR_L]

// 16-bit registers
#define BC  cache[ADDR_BC]
#define DE  cache[ADDR_DE]
#define HL  cache[ADDR_HL]
#define SP  cache[ADDR_SP]  // stack pointer
#define PC  cache[ADDR_PC]  // program counter

// Flag register (F) bits:
#define F_Z_BIT 7   // Zero Flag
#define F_N_BIT 6   // Subtract Flag
#define F_H_BIT 5   // Half Carry Flag
#define F_C_BIT 4   // Carry Flag
#define F_Z     F>>F_Z_BIT&0x01
#define F_N     F>>F_N_BIT&0x01
#define F_H     F>>F_H_BIT&0x01
#define F_C     F>>F_C_BIT&0x01

#define Flag_zero(n) do {\
    if(n == 0) {\
        F |= 1<<F_Z_BIT;\
    }\
} while(0)

#define Flag_subtract() do {\
    F |= 1<<F_N_BIT;\
} while(0)

#define Flag_carry(n) do {\
    if(n > 255) {\
        F |= 1<<F_C_BIT;\
    }\
} while(0)\

#define Flag_hcarry(n) do {\
    if(n > 15) {\
        F |= 1<<F_H_BIT;\
    }\
} while(0)\

WASM_EXPORT

void debug() {
    printf("*--------------------------*\n");
    printf("Z	N	H	C	0	0	0	0\n");
    printf("%d	%d	%d	%d	0	0	0	0\n", F_Z, F_N, F_H, F_C);
    printf("*--------------------------*\n");
    printf("| A | F |\t00:\t%04X\n", cache[0]);
    printf("| B | C |\t02:\t%04X\n", cache[1]);
    printf("| D | E |\t04:\t%04X\n", cache[2]);
    printf("| H | L |\t06:\t%04X\n", cache[3]);
    printf("| S - P |\t08:\t%04X\n", cache[4]);
    printf("| P - C |\t0A:\t%04X\n", cache[5]);
    printf("\n");
}

int main()
{
    F = 0xF;
    A = 20;
    B = 10;
    C = 0xC;
    D = 0xD;
    E = 0xE;
    H = 0x1;
    L = 0x2;
    SP = 0x0F;
    PC = 0xAAF0;
    debug();
    sub_A_B();
    debug();
    return 0;
}



static unsigned char ft;
static unsigned char ct;

void (*CB[0x100])() = {};

static void NOP() {
    ft = 4;
}

void (*op_map[])() = {
    NOP
};

unsigned char ROM[0xFF];

void exe() {
    op_map[ROM[PC++]]();
    ct += ft;
    ft = 0;
}


#define _d8     (ROM[PC++])
#define _d16    (ROM[PC++]|ROM[PC++]<<8)
#define _a8     (ROM[0xFF00|_d8])
#define _a16    (ROM[_d16])

// NOTE: 8-Bit Loads
// ld r, n
static void ld_A_d8() {A = _d8;ft = 8;}
static void ld_B_d8() {B = _d8;ft = 8;}
static void ld_C_d8() {C = _d8;ft = 8;}
static void ld_D_d8() {D = _d8;ft = 8;}
static void ld_E_d8() {E = _d8;ft = 8;}
static void ld_H_d8() {H = _d8;ft = 8;}
static void ld_L_d8() {L = _d8;ft = 8;}

// ld r, r'
static void ld_A_A() {A = A;ft = 4;}
static void ld_A_B() {A = B;ft = 4;}
static void ld_A_C() {A = C;ft = 4;}
static void ld_A_D() {A = D;ft = 4;}
static void ld_A_E() {A = E;ft = 4;}
static void ld_A_H() {A = H;ft = 4;}
static void ld_A_L() {A = L;ft = 4;}

// ld r, r'
static void ld_B_A() {B = A;ft = 4;}
static void ld_B_B() {B = B;ft = 4;}
static void ld_B_C() {B = C;ft = 4;}
static void ld_B_D() {B = D;ft = 4;}
static void ld_B_E() {B = E;ft = 4;}
static void ld_B_H() {B = H;ft = 4;}
static void ld_B_L() {B = L;ft = 4;}

// ld r, r'
static void ld_C_A() {C = A;ft = 4;}
static void ld_C_B() {C = B;ft = 4;}
static void ld_C_C() {C = C;ft = 4;}
static void ld_C_D() {C = D;ft = 4;}
static void ld_C_E() {C = E;ft = 4;}
static void ld_C_H() {C = H;ft = 4;}
static void ld_C_L() {C = L;ft = 4;}

// ld r, r'
static void ld_D_A() {D = A;ft = 4;}
static void ld_D_B() {D = B;ft = 4;}
static void ld_D_C() {D = C;ft = 4;}
static void ld_D_D() {D = D;ft = 4;}
static void ld_D_E() {D = E;ft = 4;}
static void ld_D_H() {D = H;ft = 4;}
static void ld_D_L() {D = L;ft = 4;}

// ld r, r'
static void ld_E_A() {E = A;ft = 4;}
static void ld_E_B() {E = B;ft = 4;}
static void ld_E_C() {E = C;ft = 4;}
static void ld_E_D() {E = D;ft = 4;}
static void ld_E_E() {E = E;ft = 4;}
static void ld_E_H() {E = H;ft = 4;}
static void ld_E_L() {E = L;ft = 4;}

// ld r, r'
static void ld_H_A() {H = A;ft = 4;}
static void ld_H_B() {H = B;ft = 4;}
static void ld_H_C() {H = C;ft = 4;}
static void ld_H_D() {H = D;ft = 4;}
static void ld_H_E() {H = E;ft = 4;}
static void ld_H_H() {H = H;ft = 4;}
static void ld_H_L() {H = L;ft = 4;}

// ld r, r'
static void ld_L_A() {L = A;ft = 4;}
static void ld_L_B() {L = B;ft = 4;}
static void ld_L_C() {L = C;ft = 4;}
static void ld_L_D() {L = D;ft = 4;}
static void ld_L_E() {L = E;ft = 4;}
static void ld_L_H() {L = H;ft = 4;}
static void ld_L_L() {L = L;ft = 4;}

// LD r, (HL)
static void ld_B_HL() {B = ROM[HL];ft = 8;}
static void ld_C_HL() {C = ROM[HL];ft = 8;}
static void ld_D_HL() {D = ROM[HL];ft = 8;}
static void ld_E_HL() {E = ROM[HL];ft = 8;}
static void ld_H_HL() {H = ROM[HL];ft = 8;}
static void ld_L_HL() {L = ROM[HL];ft = 8;}


// LD (HL), r
static void ld_HL_A() {ROM[HL] = A;ft = 8;}
static void ld_HL_B() {ROM[HL] = B;ft = 8;}
static void ld_HL_C() {ROM[HL] = C;ft = 8;}
static void ld_HL_D() {ROM[HL] = D;ft = 8;}
static void ld_HL_E() {ROM[HL] = E;ft = 8;}
static void ld_HL_H() {ROM[HL] = H;ft = 8;}
static void ld_HL_L() {ROM[HL] = L;ft = 8;}
static void ld_HL_d8() {ROM[HL] = _d8;ft = 12;}

// LD A, n
// n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn),#
// nn = two byte immediate value. (LS byte first.)
static void ld_A_a16() {A = _a16;ft = 16;}
static void ld_A_BC() {A = ROM[BC];ft = 8;}
static void ld_A_DE() {A = ROM[DE];ft = 8;}
static void ld_A_HL() {A = ROM[HL];ft = 8;}

// LD n, A
static void ld_a16_A() {_a16 = A;ft = 16;}
static void ld_BC_A() {ROM[BC] = A;ft = 8;}
static void ld_DE_A() {ROM[DE] = A;ft = 8;}

// LD A, (C)
// Same as: LD A, ($FF00+C)
static void ld_A_rC() {A = ROM[0xFF00|C];ft = 8;}
// LD (C), A
static void ld_rC_A() {ROM[0xFF00|C] = A;ft = 8;}

// LD A, (HL[x])    x: [I]ncrement / [D]ecrement
static void ld_A_HLD() {A = ROM[HL--];ft = 8;}
static void ld_A_HLI() {A = ROM[HL++];ft = 8;}
// LD (HL[x]), A    x: [I]ncrement / [D]ecrement
static void ld_HLD_A() {ROM[HL--] = A;ft = 8;}
static void ld_HLI_A() {ROM[HL++] = A;ft = 8;}

// LDH (n), A
static void ld_a8_A() {_a8 = A;ft = 12;}
// LDH A, (n)
static void ld_A_a8() {A = _a8;ft = 12;}


// NOTE: 16-Bit Loads
// LD n, nn
static void ld_BC_d16() {BC = _d16;ft = 12;}

// LD SP, HL
static void ld_SP_HL() {SP = HL;ft = 8;}

// LD HL, SP+n  (AKA LDHL SP, n)
static void ldhl_SP_d8() {HL = SP + _d8;ft = 12;F &= 0x3F;}

// LD (nn), SP
static void ld_a16_SP() {_a16 = SP;ft = 20;}

// PUSH nn
static void push_AF() {ROM[SP--] = A;ROM[SP--] = F;ft = 16;}
static void push_BC() {ROM[SP--] = B;ROM[SP--] = C;ft = 16;}
static void push_DE() {ROM[SP--] = D;ROM[SP--] = E;ft = 16;}
static void push_HL() {ROM[SP--] = H;ROM[SP--] = L;ft = 16;}

// POP nn
static void pop_AF() {F = ROM[SP++];A = ROM[SP++];ft = 12;}
static void pop_BC() {C = ROM[SP++];B = ROM[SP++];ft = 12;}
static void pop_DE() {E = ROM[SP++];D = ROM[SP++];ft = 12;}
static void pop_HL() {L = ROM[SP++];H = ROM[SP++];ft = 12;}

// NOTE: 8-Bit ALU

#define ADD_FLAG(n) do {\
    F = 0;\
    Flag_zero(n);\
    Flag_carry(n);\
    Flag_hcarry(n);\
} while(0)

// ADD A, n
static unsigned int sum;
static void add_A_A() {ADD_FLAG((sum=A+A));A=sum;ft = 4;}
static void add_A_B() {ADD_FLAG((sum=A+B));A=sum;ft = 4;}
static void add_A_C() {ADD_FLAG((sum=A+C));A=sum;ft = 4;}
static void add_A_D() {ADD_FLAG((sum=A+D));A=sum;ft = 4;}
static void add_A_E() {ADD_FLAG((sum=A+E));A=sum;ft = 4;}
static void add_A_H() {ADD_FLAG((sum=A+H));A=sum;ft = 4;}
static void add_A_L() {ADD_FLAG((sum=A+L));A=sum;ft = 4;}
static void add_A_HL() {ADD_FLAG((sum=A+ROM[HL]));A=sum;ft = 8;}
static void add_A_d8() {ADD_FLAG((sum=A+_d8));A=(unsigned char)sum;ft = 8;}

// ADC A, n
// Add n + Carry flag to A; (n = A,B,C,D,E,H,L,(HL),#)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Set if carry from bit 3.
//  C - Set if carry from bit 7
static void adc_A_A() {ADD_FLAG((sum=F_C+A+A));A=sum;ft = 4;}
static void adc_A_B() {ADD_FLAG((sum=F_C+A+B));A=sum;ft = 4;}
static void adc_A_C() {ADD_FLAG((sum=F_C+A+C));A=sum;ft = 4;}
static void adc_A_D() {ADD_FLAG((sum=F_C+A+D));A=sum;ft = 4;}
static void adc_A_E() {ADD_FLAG((sum=F_C+A+E));A=sum;ft = 4;}
static void adc_A_H() {ADD_FLAG((sum=F_C+A+H));A=sum;ft = 4;}
static void adc_A_L() {ADD_FLAG((sum=F_C+A+L));A=sum;ft = 4;}
static void adc_A_HL() {ADD_FLAG((sum=F_C+A+ROM[HL]));A=sum;ft = 8;}
static void adc_A_d8() {ADD_FLAG((sum=F_C+A+_d8));A=(unsigned char)sum;ft = 8;}

#define SUB_FLAG() do {\
    F = 0;\
    Flag_subtract();\
    Flag_zero(sub);\
    if(sub<0) {\
        F |= 1<<F_C_BIT;\
        sub = ~sub + 1;\
    }\
} while(0)

static int sub;

// SUB n
// Subtract n from A; (n = A,B,C,D,E,H,L,(HL),#)
// Flags affected:
//  Z - Set if result is zero.
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Set if no borrow.
static void sub_A_A() {sub=A-A;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_B() {sub=A-B;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_C() {sub=A-C;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_D() {sub=A-D;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_E() {sub=A-E;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_H() {sub=A-H;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_L() {sub=A-L;SUB_FLAG();A=sub;ft = 4;}
static void sub_A_HL() {sub=A-ROM[HL];SUB_FLAG();A=sub;ft = 8;}
static void sub_A_d8() {sub=A-_d8;SUB_FLAG();A=(unsigned char)sub;ft = 8;}

// SBC A,n
// Subtract n + Carry flag from A; (n = A,B,C,D,E,H,L,(HL),#)
static void sbc_A_A() {sub=A-F_C-A;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_B() {sub=A-F_C-B;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_C() {sub=A-F_C-C;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_D() {sub=A-F_C-D;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_E() {sub=A-F_C-E;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_H() {sub=A-F_C-H;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_L() {sub=A-F_C-L;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_HL() {sub=A-F_C-ROM[HL];SUB_FLAG();A=sub;ft = 8;}
static void sbc_A_d8() {sub=A-F_C-_d8;SUB_FLAG();A=(unsigned char)sub;ft = 8;}

// AND n
// Logically AND n with A, result in A; (n = A,B,C,D,E,H,L,(HL),#)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Set.
//  C - Reset.

#define AND_FLAG(n) do {\
    F = 0;\
    Flag_zero(n);\
    F |= 1<<F_H_BIT;\
} while(0)

static void and_A_A() {AND_FLAG((A&=A));ft = 4;}
static void and_A_B() {AND_FLAG((A&=B));ft = 4;}
static void and_A_C() {AND_FLAG((A&=C));ft = 4;}
static void and_A_D() {AND_FLAG((A&=D));ft = 4;}
static void and_A_E() {AND_FLAG((A&=E));ft = 4;}
static void and_A_H() {AND_FLAG((A&=H));ft = 4;}
static void and_A_L() {AND_FLAG((A&=L));ft = 4;}
static void and_A_HL() {AND_FLAG((A&=ROM[HL]));ft = 8;}
static void and_A_d8() {AND_FLAG((A&=_d8));ft = 8;}

// OR n
// Logical OR n with register A, result in A; (n = A,B,C,D,E,H,L,(HL),#)
// Flags affected:
//  Z - Set if result is zero.
//  N - Reset.
//  H - Reset.
//  C - Reset.

// Reset & Zero detect
#define OR_FLAG(n) do {\
    F = 0;\
    Flag_zero(n);\
} while(0)

static void or_A_A() {OR_FLAG((A|=A));ft = 4;}
static void or_A_B() {OR_FLAG((A|=B));ft = 4;}
static void or_A_C() {OR_FLAG((A|=C));ft = 4;}
static void or_A_D() {OR_FLAG((A|=D));ft = 4;}
static void or_A_E() {OR_FLAG((A|=E));ft = 4;}
static void or_A_H() {OR_FLAG((A|=H));ft = 4;}
static void or_A_L() {OR_FLAG((A|=L));ft = 4;}
static void or_A_HL() {OR_FLAG((A|=ROM[HL]));ft = 8;}
static void or_A_d8() {OR_FLAG((A|=_d8));ft = 8;}

// XOR n
// Logical exclusive OR n with register A, result in A; (n = A,B,C,D,E,H,L,(HL),#)
static void xor_A_A() {OR_FLAG((A^=A));ft = 4;}
static void xor_A_B() {OR_FLAG((A^=B));ft = 4;}
static void xor_A_C() {OR_FLAG((A^=C));ft = 4;}
static void xor_A_D() {OR_FLAG((A^=D));ft = 4;}
static void xor_A_E() {OR_FLAG((A^=E));ft = 4;}
static void xor_A_H() {OR_FLAG((A^=H));ft = 4;}
static void xor_A_L() {OR_FLAG((A^=L));ft = 4;}
static void xor_A_HL() {OR_FLAG((A^=ROM[HL]));ft = 8;}
static void xor_A_d8() {OR_FLAG((A^=_d8));ft = 8;}

// CP n
// Compare A with n. This is basically an A - n subtraction instruction but the results are thrown away.
// (n = A,B,C,D,E,H,L,(HL),#)
// Flags affected:
//  Z - Set if result is zero. (Set if A = n.)
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Set for no borrow. (Set if A < n.)
static void cp_A_A() {sub=A-A;SUB_FLAG();ft = 4;}
static void cp_A_B() {sub=A-B;SUB_FLAG();ft = 4;}
static void cp_A_C() {sub=A-C;SUB_FLAG();ft = 4;}
static void cp_A_D() {sub=A-D;SUB_FLAG();ft = 4;}
static void cp_A_E() {sub=A-E;SUB_FLAG();ft = 4;}
static void cp_A_H() {sub=A-H;SUB_FLAG();ft = 4;}
static void cp_A_L() {sub=A-L;SUB_FLAG();ft = 4;}
static void cp_A_HL() {sub=A-ROM[HL];SUB_FLAG();ft = 8;}
static void cp_A_d8() {sub=A-_d8;SUB_FLAG();ft = 8;}

// INC n
// Increment register n; (n = A,B,C,D,E,H,L,(HL))

static void inc_A() {OR_FLAG(++A);ft = 4;}
static void inc_B() {OR_FLAG(++B);ft = 4;}
static void inc_C() {OR_FLAG(++C);ft = 4;}
static void inc_D() {OR_FLAG(++D);ft = 4;}
static void inc_E() {OR_FLAG(++E);ft = 4;}
static void inc_H() {OR_FLAG(++H);ft = 4;}
static void inc_L() {OR_FLAG(++L);ft = 4;}
static void inc_rHL() {OR_FLAG(++ROM[HL]);ft = 12;}

// DEC n
// Decrement register n; (n = A,B,C,D,E,H,L,(HL))
// Flags affected:
//  Z - Set if reselt is zero.
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Not affected.
#define DEC_FLAG(n) do {\
    F = 0;\
    F |= 1<<F_N_BIT;\
    Flag_zero(n);\
} while(0)

static void dec_A() {DEC_FLAG(--A);ft = 4;}
static void dec_B() {DEC_FLAG(--B);ft = 4;}
static void dec_C() {DEC_FLAG(--C);ft = 4;}
static void dec_D() {DEC_FLAG(--D);ft = 4;}
static void dec_E() {DEC_FLAG(--E);ft = 4;}
static void dec_H() {DEC_FLAG(--H);ft = 4;}
static void dec_L() {DEC_FLAG(--L);ft = 4;}
static void dec_rHL() {DEC_FLAG(--ROM[HL]);ft = 12;}

// NOTE: 16-Bit Arithmetic

// ADD HL,n
// Add n to HL; (n = BC,DE,HL,SP)
// Flags affected:
//  Z - Not affected.
//  N - Reset.
//  H - Set if carry from bit 11.
//  C - Set if carry from bit 15.

// 2 bytes add flag
#define ADD_FLAG_2B(n) do {\
    F = 0;\
    Flag_zero(n);\
    if(n > 0xFFFF) {\
        F |= 1<<F_C_BIT;\
    }\
} while(0)

static void add_HL_BC() {ADD_FLAG_2B((sum=HL+BC));HL=sum;ft = 8;}
static void add_HL_DE() {ADD_FLAG_2B((sum=HL+DE));HL=sum;ft = 8;}
static void add_HL_HL() {ADD_FLAG_2B((sum=HL+HL));HL=sum;ft = 8;}
static void add_HL_SP() {ADD_FLAG_2B((sum=HL+SP));HL=sum;ft = 8;}

// ADD SP,n
// Add n to Stack Pointer (SP); (n = one byte signed immediate value (#))

static void add_SP_d8() {SP+=(signed char)_d8;F=0;ft = 16;}

// INC nn
// Increment register nn; (nn = BC,DE,HL,SP)
// None flags affected
static void inc_BC() {BC++;ft = 8;}
static void inc_DE() {DE++;ft = 8;}
static void inc_HL() {HL++;ft = 8;}
static void inc_SP() {SP++;ft = 8;}

// DEC nn
// Decrement register nn
static void dec_BC() {BC--;ft = 8;}
static void dec_DE() {DE--;ft = 8;}
static void dec_HL() {HL--;ft = 8;}
static void dec_SP() {SP--;ft = 8;}

// NOTE: Miscellaneous
static unsigned char swap;
// SWAP n
// Swap upper & lower nibles of n; (n = A,B,C,D,E,H,L,(HL))
static void swap_A() {swap=A&0xF0;A=A<<4|swap>>4;OR_FLAG(A);ft = 8;}
static void swap_B() {swap=B&0xF0;B=B<<4|swap>>4;OR_FLAG(B);ft = 8;}
static void swap_C() {swap=C&0xF0;C=C<<4|swap>>4;OR_FLAG(C);ft = 8;}
static void swap_D() {swap=D&0xF0;D=D<<4|swap>>4;OR_FLAG(D);ft = 8;}
static void swap_E() {swap=E&0xF0;E=E<<4|swap>>4;OR_FLAG(E);ft = 8;}
static void swap_H() {swap=H&0xF0;H=H<<4|swap>>4;OR_FLAG(H);ft = 8;}
static void swap_L() {swap=L&0xF0;L=L<<4|swap>>4;OR_FLAG(L);ft = 8;}
static void swap_HL() {swap=ROM[HL]&0xF0;ROM[HL]=ROM[HL]<<4|swap>>4;OR_FLAG(ROM[HL]);ft = 16;}

// DAA


