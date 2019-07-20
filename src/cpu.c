#include "cpu.h"
#include "mmu.h"
#include <stdio.h>

#define REG_AMOUNT 6

static unsigned short cache[REG_AMOUNT] = {};

// 1 BYTE REG
#define ADDR_F  0
#define ADDR_A  1
#define ADDR_C  2
#define ADDR_B  3
#define ADDR_E  4
#define ADDR_D  5
#define ADDR_L  6
#define ADDR_H  7

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

void cpu_debug() {
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


// 4Mhz
unsigned char ft;
unsigned int ct;

// void (*CB[0x100])() = {};

// unsigned char MEM(0xFF];




#define _d8     (MEM(PC++))
#define _d16    (MEM(PC++)|MEM(PC++)<<8)
#define _a8     (MEM(0xFF00|_d8))
#define _a16    (MEM(_d16))

static void XX() {
    const unsigned short loc = PC-1;
    printf("Unimplemented instruction at $%04X\n", PC);
}

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
static void ld_B_HL() {B = MEM(HL);ft = 8;}
static void ld_C_HL() {C = MEM(HL);ft = 8;}
static void ld_D_HL() {D = MEM(HL);ft = 8;}
static void ld_E_HL() {E = MEM(HL);ft = 8;}
static void ld_H_HL() {H = MEM(HL);ft = 8;}
static void ld_L_HL() {L = MEM(HL);ft = 8;}


// LD (HL), r
static void ld_HL_A() {MEM(HL) = A;ft = 8;}
static void ld_HL_B() {MEM(HL) = B;ft = 8;}
static void ld_HL_C() {MEM(HL) = C;ft = 8;}
static void ld_HL_D() {MEM(HL) = D;ft = 8;}
static void ld_HL_E() {MEM(HL) = E;ft = 8;}
static void ld_HL_H() {MEM(HL) = H;ft = 8;}
static void ld_HL_L() {MEM(HL) = L;ft = 8;}
static void ld_HL_d8() {MEM(HL) = _d8;ft = 12;}

// LD A, n
// n = A,B,C,D,E,H,L,(BC),(DE),(HL),(nn),#
// nn = two byte immediate value. (LS byte first.)
static void ld_A_a16() {A = _a16;ft = 16;}
static void ld_A_BC() {A = MEM(BC);ft = 8;}
static void ld_A_DE() {A = MEM(DE);ft = 8;}
static void ld_A_HL() {A = MEM(HL);ft = 8;}

// LD n, A
static void ld_a16_A() {_a16 = A;ft = 16;}
static void ld_BC_A() {MEM(BC) = A;ft = 8;}
static void ld_DE_A() {MEM(DE) = A;ft = 8;}

// LD A, (C)
// Same as: LD A, ($FF00+C)
static void ld_A_rC() {A = MEM(0xFF00|C);ft = 8;}
// LD (C), A
static void ld_rC_A() {MEM(0xFF00|C) = A;ft = 8;}

// LD A, (HL[x])    x: [I]ncrement / [D]ecrement
static void ld_A_HLD() {A = MEM(HL--);ft = 8;}
static void ld_A_HLI() {A = MEM(HL++);ft = 8;}
// LD (HL[x]), A    x: [I]ncrement / [D]ecrement
static void ld_HLD_A() {MEM(HL--) = A;ft = 8;}
static void ld_HLI_A() {MEM(HL++) = A;ft = 8;}

// LDH (n), A
static void ld_a8_A() {_a8 = A;ft = 12;}
// LDH A, (n)
static void ld_A_a8() {A = _a8;ft = 12;}


// NOTE: 16-Bit Loads

// LD n, nn
// n = BC,DE,HL,SP, nn = 16 bit immediate value
static void ld_BC_d16() {BC = _d16;ft = 12;}
static void ld_DE_d16() {DE = _d16;ft = 12;}
static void ld_HL_d16() {HL = _d16;ft = 12;}
static void ld_SP_d16() {SP = _d16;ft = 12;}

// LD SP, HL
static void ld_SP_HL() {SP = HL;ft = 8;}

// LD HL, SP+n  (AKA LDHL SP, n)
static void ldhl_SP_d8() {HL = SP + _d8;ft = 12;F &= 0x3F;}

// LD (nn), SP
static void ld_a16_SP() {_a16 = SP;ft = 20;}

// PUSH nn
static void push_AF() {MEM(--SP) = A;MEM(--SP) = F;ft = 16;}
static void push_BC() {MEM(--SP) = B;MEM(--SP) = C;ft = 16;}
static void push_DE() {MEM(--SP) = D;MEM(--SP) = E;ft = 16;}
static void push_HL() {MEM(--SP) = H;MEM(--SP) = L;ft = 16;}

// POP nn
static void pop_AF() {F = MEM(SP++);A = MEM(SP++);ft = 12;}
static void pop_BC() {C = MEM(SP++);B = MEM(SP++);ft = 12;}
static void pop_DE() {E = MEM(SP++);D = MEM(SP++);ft = 12;}
static void pop_HL() {L = MEM(SP++);H = MEM(SP++);ft = 12;}

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
static void add_A_HL() {ADD_FLAG((sum=A+MEM(HL)));A=sum;ft = 8;}
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
static void adc_A_HL() {ADD_FLAG((sum=F_C+A+MEM(HL)));A=sum;ft = 8;}
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
static void sub_A() {sub=A-A;SUB_FLAG();A=sub;ft = 4;}
static void sub_B() {sub=A-B;SUB_FLAG();A=sub;ft = 4;}
static void sub_C() {sub=A-C;SUB_FLAG();A=sub;ft = 4;}
static void sub_D() {sub=A-D;SUB_FLAG();A=sub;ft = 4;}
static void sub_E() {sub=A-E;SUB_FLAG();A=sub;ft = 4;}
static void sub_H() {sub=A-H;SUB_FLAG();A=sub;ft = 4;}
static void sub_L() {sub=A-L;SUB_FLAG();A=sub;ft = 4;}
static void sub_HL() {sub=A-MEM(HL);SUB_FLAG();A=sub;ft = 8;}
static void sub_d8() {sub=A-_d8;SUB_FLAG();A=(unsigned char)sub;ft = 8;}

// SBC A,n
// Subtract n + Carry flag from A; (n = A,B,C,D,E,H,L,(HL),#)
static void sbc_A_A() {sub=A-F_C-A;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_B() {sub=A-F_C-B;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_C() {sub=A-F_C-C;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_D() {sub=A-F_C-D;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_E() {sub=A-F_C-E;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_H() {sub=A-F_C-H;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_L() {sub=A-F_C-L;SUB_FLAG();A=sub;ft = 4;}
static void sbc_A_HL() {sub=A-F_C-MEM(HL);SUB_FLAG();A=sub;ft = 8;}
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

static void and_A() {AND_FLAG((A&=A));ft = 4;}
static void and_B() {AND_FLAG((A&=B));ft = 4;}
static void and_C() {AND_FLAG((A&=C));ft = 4;}
static void and_D() {AND_FLAG((A&=D));ft = 4;}
static void and_E() {AND_FLAG((A&=E));ft = 4;}
static void and_H() {AND_FLAG((A&=H));ft = 4;}
static void and_L() {AND_FLAG((A&=L));ft = 4;}
static void and_HL() {AND_FLAG((A&=MEM(HL)));ft = 8;}
static void and_d8() {AND_FLAG((A&=_d8));ft = 8;}

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

static void or_A() {OR_FLAG((A|=A));ft = 4;}
static void or_B() {OR_FLAG((A|=B));ft = 4;}
static void or_C() {OR_FLAG((A|=C));ft = 4;}
static void or_D() {OR_FLAG((A|=D));ft = 4;}
static void or_E() {OR_FLAG((A|=E));ft = 4;}
static void or_H() {OR_FLAG((A|=H));ft = 4;}
static void or_L() {OR_FLAG((A|=L));ft = 4;}
static void or_HL() {OR_FLAG((A|=MEM(HL)));ft = 8;}
static void or_d8() {OR_FLAG((A|=_d8));ft = 8;}

// XOR n
// Logical exclusive OR n with register A, result in A; (n = A,B,C,D,E,H,L,(HL),#)
static void xor_A() {OR_FLAG((A^=A));ft = 4;}
static void xor_B() {OR_FLAG((A^=B));ft = 4;}
static void xor_C() {OR_FLAG((A^=C));ft = 4;}
static void xor_D() {OR_FLAG((A^=D));ft = 4;}
static void xor_E() {OR_FLAG((A^=E));ft = 4;}
static void xor_H() {OR_FLAG((A^=H));ft = 4;}
static void xor_L() {OR_FLAG((A^=L));ft = 4;}
static void xor_HL() {OR_FLAG((A^=MEM(HL)));ft = 8;}
static void xor_d8() {OR_FLAG((A^=_d8));ft = 8;}

// CP n
// Compare A with n. This is basically an A - n subtraction instruction but the results are thrown away.
// (n = A,B,C,D,E,H,L,(HL),#)
// Flags affected:
//  Z - Set if result is zero. (Set if A = n.)
//  N - Set.
//  H - Set if no borrow from bit 4.
//  C - Set for no borrow. (Set if A < n.)
static void cp_A() {sub=A-A;SUB_FLAG();ft = 4;}
static void cp_B() {sub=A-B;SUB_FLAG();ft = 4;}
static void cp_C() {sub=A-C;SUB_FLAG();ft = 4;}
static void cp_D() {sub=A-D;SUB_FLAG();ft = 4;}
static void cp_E() {sub=A-E;SUB_FLAG();ft = 4;}
static void cp_H() {sub=A-H;SUB_FLAG();ft = 4;}
static void cp_L() {sub=A-L;SUB_FLAG();ft = 4;}
static void cp_HL() {sub=A-MEM(HL);SUB_FLAG();ft = 8;}
static void cp_d8() {sub=A-_d8;SUB_FLAG();ft = 8;}

// INC n
// Increment register n; (n = A,B,C,D,E,H,L,(HL))

static void inc_A() {OR_FLAG(++A);ft = 4;}
static void inc_B() {OR_FLAG(++B);ft = 4;}
static void inc_C() {OR_FLAG(++C);ft = 4;}
static void inc_D() {OR_FLAG(++D);ft = 4;}
static void inc_E() {OR_FLAG(++E);ft = 4;}
static void inc_H() {OR_FLAG(++H);ft = 4;}
static void inc_L() {OR_FLAG(++L);ft = 4;}
static void inc_rHL() {OR_FLAG(++MEM(HL));ft = 12;}

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
static void dec_rHL() {DEC_FLAG(--MEM(HL));ft = 12;}

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
static void swap_HL() {swap=MEM(HL)&0xF0;MEM(HL)=MEM(HL)<<4|swap>>4;OR_FLAG(MEM(HL));ft = 16;}

// DAA TODO:

// CPL
// Complement A register. (Flip all bits.)
// Flags affected: - 1 1 -
static void cpl() {A=~A;F|=0x60;ft = 4;}

// CCF
// Complement carry flag
static void ccf() {F&=~0x60;F^=0x10;ft = 4;}

// SCF
// Set Carry flag
static void scf() {F&=~0x60;F|=0x10;ft = 4;}

// NOP
// No operation
static void nop() {ft = 4;}

// HALT
// Power down CPU until an interrupt occurs. Use this when ever possible to reduce energy consumption
static unsigned char isHalt = 0;
static void halt() {isHalt=1;ft = 4;}

// STOP
// Halt CPU & LCD display until button pressed
static unsigned char isStop = 0;
static void stop() {isStop=1;ft = 4;}

// DI
// This instruction disables interrupts but not immediately. Interrupts are disabled after instruction after DI is executed
static unsigned char IME = 0;
static void di() {IME=0;ft = 4;}
// EI
// Enable interrupts. This intruction enables interrupts but not immediately. Interrupts are enabled after instruction after EI is executed.
// NOTE: 1 machine cycle (+ 1 machine cycle for the effect)
static void ei() {IME=1;ft = 4;}


// NOTE: Rotates & Shifts
// RLCA
// Rotate A left. Old bit 7 to Carry flag
// FIXME: conflict 0 0 0 C or Z 0 0 C
#define RLCA_FLAG(n) do {\
    F = 0;\
    F |= (n & 1)<<F_C_BIT;\
    Flag_zero(n);\
} while(0)
static void rlca() {swap=A&0x80;A=(A<<1)|(swap>>7);RLCA_FLAG(A);ft = 4;}

// RLA
// Rotate A left through Carry flag
static void rla() {swap=A&0x80;A=(A<<1)|(F&0x10>>4);OR_FLAG(A);F|=swap>>3;ft = 4;}

// RRCA
// Rotate A right. Old bit 0 to Carry flag
#define RRCA_FLAG(n) do {\
    F = 0;\
    F |= (n & 0x80)>>3;\
    Flag_zero(n);\
} while(0)
static void rrca() {swap=A&1;A=(A>>1)|(swap<<7);RRCA_FLAG(A);ft = 4;}

// RRA
// Rotate A right through Carry flag
static void rra() {swap=A&1;A=(A>>1)|(F&0x10<<3);OR_FLAG(A);F|=swap<<F_C_BIT;ft = 4;}

//_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=
// RLC n
// Rotate n left through Carry flag; (n = A,B,C,D,E,H,L,(HL))
static void rlc_A() {swap=A&0x80;A=(A<<1)|(swap>>7);RLCA_FLAG(A);ft = 8;}
static void rlc_B() {swap=B&0x80;B=(B<<1)|(swap>>7);RLCA_FLAG(B);ft = 8;}
static void rlc_C() {swap=C&0x80;C=(C<<1)|(swap>>7);RLCA_FLAG(C);ft = 8;}
static void rlc_D() {swap=D&0x80;D=(D<<1)|(swap>>7);RLCA_FLAG(D);ft = 8;}
static void rlc_E() {swap=E&0x80;E=(E<<1)|(swap>>7);RLCA_FLAG(E);ft = 8;}
static void rlc_H() {swap=H&0x80;H=(H<<1)|(swap>>7);RLCA_FLAG(H);ft = 8;}
static void rlc_L() {swap=L&0x80;L=(L<<1)|(swap>>7);RLCA_FLAG(L);ft = 8;}
static void rlc_HL() {swap=MEM(HL)&0x80;MEM(HL)=(MEM(HL)<<1)|(swap>>7);RLCA_FLAG(MEM(HL));ft = 16;}

// RL n
// Rotate n left through Carry flag; (n = A,B,C,D,E,H,L,(HL))
static void rl_A() {swap=A&0x80;A=(A<<1)|(F&0x10>>4);OR_FLAG(A);F|=swap>>3;ft = 4;}
static void rl_B() {swap=B&0x80;B=(B<<1)|(F&0x10>>4);OR_FLAG(B);F|=swap>>3;ft = 4;}
static void rl_C() {swap=C&0x80;C=(C<<1)|(F&0x10>>4);OR_FLAG(C);F|=swap>>3;ft = 4;}
static void rl_D() {swap=D&0x80;D=(D<<1)|(F&0x10>>4);OR_FLAG(D);F|=swap>>3;ft = 4;}
static void rl_E() {swap=E&0x80;E=(E<<1)|(F&0x10>>4);OR_FLAG(E);F|=swap>>3;ft = 4;}
static void rl_H() {swap=H&0x80;H=(H<<1)|(F&0x10>>4);OR_FLAG(H);F|=swap>>3;ft = 4;}
static void rl_L() {swap=L&0x80;L=(L<<1)|(F&0x10>>4);OR_FLAG(L);F|=swap>>3;ft = 4;}
static void rl_HL() {swap=MEM(HL)&0x80;MEM(HL)=(MEM(HL)<<1)|(F&0x10>>4);OR_FLAG(MEM(HL));F|=swap>>3;ft = 16;}

// RRC n
// Rotate n right. Old bit 0 to Carry flag
static void rrc_A() {swap=A&1;A=(A>>1)|(swap<<7);RRCA_FLAG(A);ft = 4;}
static void rrc_B() {swap=B&1;B=(B>>1)|(swap<<7);RRCA_FLAG(B);ft = 4;}
static void rrc_C() {swap=C&1;C=(C>>1)|(swap<<7);RRCA_FLAG(C);ft = 4;}
static void rrc_D() {swap=D&1;D=(D>>1)|(swap<<7);RRCA_FLAG(D);ft = 4;}
static void rrc_E() {swap=E&1;E=(E>>1)|(swap<<7);RRCA_FLAG(E);ft = 4;}
static void rrc_H() {swap=H&1;H=(H>>1)|(swap<<7);RRCA_FLAG(H);ft = 4;}
static void rrc_L() {swap=L&1;L=(L>>1)|(swap<<7);RRCA_FLAG(L);ft = 4;}
static void rrc_HL() {swap=MEM(HL)&1;MEM(HL)=(MEM(HL)>>1)|(swap<<7);RRCA_FLAG(MEM(HL));ft = 16;}

// RR n
// Rotate n right through Carry flag
static void rr_A() {swap=A&1;A=(A>>1)|(F&0x10<<3);OR_FLAG(A);F|=swap<<F_C_BIT;ft = 4;}
static void rr_B() {swap=B&1;B=(B>>1)|(F&0x10<<3);OR_FLAG(B);F|=swap<<F_C_BIT;ft = 4;}
static void rr_C() {swap=C&1;C=(C>>1)|(F&0x10<<3);OR_FLAG(C);F|=swap<<F_C_BIT;ft = 4;}
static void rr_D() {swap=D&1;D=(D>>1)|(F&0x10<<3);OR_FLAG(D);F|=swap<<F_C_BIT;ft = 4;}
static void rr_E() {swap=E&1;E=(E>>1)|(F&0x10<<3);OR_FLAG(E);F|=swap<<F_C_BIT;ft = 4;}
static void rr_H() {swap=H&1;H=(H>>1)|(F&0x10<<3);OR_FLAG(H);F|=swap<<F_C_BIT;ft = 4;}
static void rr_L() {swap=L&1;L=(L>>1)|(F&0x10<<3);OR_FLAG(L);F|=swap<<F_C_BIT;ft = 4;}
static void rr_HL() {swap=MEM(HL)&1;MEM(HL)=(MEM(HL)>>1)|(F&0x10<<3);OR_FLAG(MEM(HL));F|=swap<<F_C_BIT;ft = 16;}

// SLA n
// Shift n left into Carry. LSB of n set to 0; (n = A,B,C,D,E,H,L,(HL))
static void sla_A() {swap=A;A=(A<<1);OR_FLAG(A);F|=swap&0x80>>3;ft = 8;}
static void sla_B() {swap=B;B=(B<<1);OR_FLAG(B);F|=swap&0x80>>3;ft = 8;}
static void sla_C() {swap=C;C=(C<<1);OR_FLAG(C);F|=swap&0x80>>3;ft = 8;}
static void sla_D() {swap=D;D=(D<<1);OR_FLAG(D);F|=swap&0x80>>3;ft = 8;}
static void sla_E() {swap=E;E=(E<<1);OR_FLAG(E);F|=swap&0x80>>3;ft = 8;}
static void sla_H() {swap=H;H=(H<<1);OR_FLAG(H);F|=swap&0x80>>3;ft = 8;}
static void sla_L() {swap=L;L=(L<<1);OR_FLAG(L);F|=swap&0x80>>3;ft = 8;}
static void sla_HL() {swap=MEM(HL);MEM(HL)=(MEM(HL)<<1);OR_FLAG(MEM(HL));F|=swap&0x80>>3;ft = 16;}

// SRA n
// Shift n right into Carry. MSB doesn't change
static void sra_A() {swap=A;A=(A>>1)|(swap&0x80);OR_FLAG(A);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_B() {swap=B;B=(B>>1)|(swap&0x80);OR_FLAG(B);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_C() {swap=C;C=(C>>1)|(swap&0x80);OR_FLAG(C);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_D() {swap=D;D=(D>>1)|(swap&0x80);OR_FLAG(D);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_E() {swap=E;E=(E>>1)|(swap&0x80);OR_FLAG(E);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_H() {swap=H;H=(H>>1)|(swap&0x80);OR_FLAG(H);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_L() {swap=L;L=(L>>1)|(swap&0x80);OR_FLAG(L);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void sra_HL() {swap=MEM(HL);MEM(HL)=(MEM(HL)>>1)|(swap&0x80);OR_FLAG(MEM(HL));F|=(swap&1)<<F_C_BIT;ft = 16;}

// SRL n
// Shift n right into Carry. MSB set to 0
static void srl_A() {swap=A;A=(A>>1);OR_FLAG(A);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_B() {swap=B;B=(B>>1);OR_FLAG(B);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_C() {swap=C;C=(C>>1);OR_FLAG(C);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_D() {swap=D;D=(D>>1);OR_FLAG(D);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_E() {swap=E;E=(E>>1);OR_FLAG(E);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_H() {swap=H;H=(H>>1);OR_FLAG(H);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_L() {swap=L;L=(L>>1);OR_FLAG(L);F|=(swap&1)<<F_C_BIT;ft = 8;}
static void srl_HL() {swap=MEM(HL);MEM(HL)=(MEM(HL)>>1);OR_FLAG(MEM(HL));F|=(swap&1)<<F_C_BIT;ft = 16;}


// BIT b,r
// Test bit b in register r; (b = 0 - 7, r = A,B,C,D,E,H,L,(HL))
// Flags affected:
//  Z - Set if bit b of register r is 0.
//  N - Reset.
//  H - Set.
//  C - Not affected.
#define BIT_FLAG(b, r) do {\
    F |= 1<<F_H_BIT;\
    F &= ~(1<<F_N_BIT);\
    Flag_zero(r&~(1<<b));\
} while(0)

static void bit_0_A() {BIT_FLAG(0,A);ft = 8;}
static void bit_0_B() {BIT_FLAG(0,B);ft = 8;}
static void bit_0_C() {BIT_FLAG(0,C);ft = 8;}
static void bit_0_D() {BIT_FLAG(0,D);ft = 8;}
static void bit_0_E() {BIT_FLAG(0,E);ft = 8;}
static void bit_0_H() {BIT_FLAG(0,H);ft = 8;}
static void bit_0_L() {BIT_FLAG(0,L);ft = 8;}
static void bit_0_HL() {BIT_FLAG(0,MEM(HL));ft = 16;}

static void bit_1_A() {BIT_FLAG(1,A);ft = 8;}
static void bit_1_B() {BIT_FLAG(1,B);ft = 8;}
static void bit_1_C() {BIT_FLAG(1,C);ft = 8;}
static void bit_1_D() {BIT_FLAG(1,D);ft = 8;}
static void bit_1_E() {BIT_FLAG(1,E);ft = 8;}
static void bit_1_H() {BIT_FLAG(1,H);ft = 8;}
static void bit_1_L() {BIT_FLAG(1,L);ft = 8;}
static void bit_1_HL() {BIT_FLAG(1,MEM(HL));ft = 16;}

static void bit_2_A() {BIT_FLAG(2,A);ft = 8;}
static void bit_2_B() {BIT_FLAG(2,B);ft = 8;}
static void bit_2_C() {BIT_FLAG(2,C);ft = 8;}
static void bit_2_D() {BIT_FLAG(2,D);ft = 8;}
static void bit_2_E() {BIT_FLAG(2,E);ft = 8;}
static void bit_2_H() {BIT_FLAG(2,H);ft = 8;}
static void bit_2_L() {BIT_FLAG(2,L);ft = 8;}
static void bit_2_HL() {BIT_FLAG(2,MEM(HL));ft = 16;}

static void bit_3_A() {BIT_FLAG(3,A);ft = 8;}
static void bit_3_B() {BIT_FLAG(3,B);ft = 8;}
static void bit_3_C() {BIT_FLAG(3,C);ft = 8;}
static void bit_3_D() {BIT_FLAG(3,D);ft = 8;}
static void bit_3_E() {BIT_FLAG(3,E);ft = 8;}
static void bit_3_H() {BIT_FLAG(3,H);ft = 8;}
static void bit_3_L() {BIT_FLAG(3,L);ft = 8;}
static void bit_3_HL() {BIT_FLAG(3,MEM(HL));ft = 16;}

static void bit_4_A() {BIT_FLAG(4,A);ft = 8;}
static void bit_4_B() {BIT_FLAG(4,B);ft = 8;}
static void bit_4_C() {BIT_FLAG(4,C);ft = 8;}
static void bit_4_D() {BIT_FLAG(4,D);ft = 8;}
static void bit_4_E() {BIT_FLAG(4,E);ft = 8;}
static void bit_4_H() {BIT_FLAG(4,H);ft = 8;}
static void bit_4_L() {BIT_FLAG(4,L);ft = 8;}
static void bit_4_HL() {BIT_FLAG(4,MEM(HL));ft = 16;}

static void bit_5_A() {BIT_FLAG(5,A);ft = 8;}
static void bit_5_B() {BIT_FLAG(5,B);ft = 8;}
static void bit_5_C() {BIT_FLAG(5,C);ft = 8;}
static void bit_5_D() {BIT_FLAG(5,D);ft = 8;}
static void bit_5_E() {BIT_FLAG(5,E);ft = 8;}
static void bit_5_H() {BIT_FLAG(5,H);ft = 8;}
static void bit_5_L() {BIT_FLAG(5,L);ft = 8;}
static void bit_5_HL() {BIT_FLAG(5,MEM(HL));ft = 16;}

static void bit_6_A() {BIT_FLAG(6,A);ft = 8;}
static void bit_6_B() {BIT_FLAG(6,B);ft = 8;}
static void bit_6_C() {BIT_FLAG(6,C);ft = 8;}
static void bit_6_D() {BIT_FLAG(6,D);ft = 8;}
static void bit_6_E() {BIT_FLAG(6,E);ft = 8;}
static void bit_6_H() {BIT_FLAG(6,H);ft = 8;}
static void bit_6_L() {BIT_FLAG(6,L);ft = 8;}
static void bit_6_HL() {BIT_FLAG(6,MEM(HL));ft = 16;}

static void bit_7_A() {BIT_FLAG(7,A);ft = 8;}
static void bit_7_B() {BIT_FLAG(7,B);ft = 8;}
static void bit_7_C() {BIT_FLAG(7,C);ft = 8;}
static void bit_7_D() {BIT_FLAG(7,D);ft = 8;}
static void bit_7_E() {BIT_FLAG(7,E);ft = 8;}
static void bit_7_H() {BIT_FLAG(7,H);ft = 8;}
static void bit_7_L() {BIT_FLAG(7,L);ft = 8;}
static void bit_7_HL() {BIT_FLAG(7,MEM(HL));ft = 16;}

// SET b,r
// Set bit b in register r; (b = 0 - 7, r = A,B,C,D,E,H,L,(HL))
#define BIT_SET(b, r) do {\
    r |= 1<<b;\
} while(0)

static void set_0_A() {BIT_SET(0,A);ft = 8;}
static void set_0_B() {BIT_SET(0,B);ft = 8;}
static void set_0_C() {BIT_SET(0,C);ft = 8;}
static void set_0_D() {BIT_SET(0,D);ft = 8;}
static void set_0_E() {BIT_SET(0,E);ft = 8;}
static void set_0_H() {BIT_SET(0,H);ft = 8;}
static void set_0_L() {BIT_SET(0,L);ft = 8;}
static void set_0_HL() {BIT_SET(0,MEM(HL));ft = 16;}

static void set_1_A() {BIT_SET(1,A);ft = 8;}
static void set_1_B() {BIT_SET(1,B);ft = 8;}
static void set_1_C() {BIT_SET(1,C);ft = 8;}
static void set_1_D() {BIT_SET(1,D);ft = 8;}
static void set_1_E() {BIT_SET(1,E);ft = 8;}
static void set_1_H() {BIT_SET(1,H);ft = 8;}
static void set_1_L() {BIT_SET(1,L);ft = 8;}
static void set_1_HL() {BIT_SET(1,MEM(HL));ft = 16;}

static void set_2_A() {BIT_SET(2,A);ft = 8;}
static void set_2_B() {BIT_SET(2,B);ft = 8;}
static void set_2_C() {BIT_SET(2,C);ft = 8;}
static void set_2_D() {BIT_SET(2,D);ft = 8;}
static void set_2_E() {BIT_SET(2,E);ft = 8;}
static void set_2_H() {BIT_SET(2,H);ft = 8;}
static void set_2_L() {BIT_SET(2,L);ft = 8;}
static void set_2_HL() {BIT_SET(2,MEM(HL));ft = 16;}

static void set_3_A() {BIT_SET(3,A);ft = 8;}
static void set_3_B() {BIT_SET(3,B);ft = 8;}
static void set_3_C() {BIT_SET(3,C);ft = 8;}
static void set_3_D() {BIT_SET(3,D);ft = 8;}
static void set_3_E() {BIT_SET(3,E);ft = 8;}
static void set_3_H() {BIT_SET(3,H);ft = 8;}
static void set_3_L() {BIT_SET(3,L);ft = 8;}
static void set_3_HL() {BIT_SET(3,MEM(HL));ft = 16;}

static void set_4_A() {BIT_SET(4,A);ft = 8;}
static void set_4_B() {BIT_SET(4,B);ft = 8;}
static void set_4_C() {BIT_SET(4,C);ft = 8;}
static void set_4_D() {BIT_SET(4,D);ft = 8;}
static void set_4_E() {BIT_SET(4,E);ft = 8;}
static void set_4_H() {BIT_SET(4,H);ft = 8;}
static void set_4_L() {BIT_SET(4,L);ft = 8;}
static void set_4_HL() {BIT_SET(4,MEM(HL));ft = 16;}

static void set_5_A() {BIT_SET(5,A);ft = 8;}
static void set_5_B() {BIT_SET(5,B);ft = 8;}
static void set_5_C() {BIT_SET(5,C);ft = 8;}
static void set_5_D() {BIT_SET(5,D);ft = 8;}
static void set_5_E() {BIT_SET(5,E);ft = 8;}
static void set_5_H() {BIT_SET(5,H);ft = 8;}
static void set_5_L() {BIT_SET(5,L);ft = 8;}
static void set_5_HL() {BIT_SET(5,MEM(HL));ft = 16;}

static void set_6_A() {BIT_SET(6,A);ft = 8;}
static void set_6_B() {BIT_SET(6,B);ft = 8;}
static void set_6_C() {BIT_SET(6,C);ft = 8;}
static void set_6_D() {BIT_SET(6,D);ft = 8;}
static void set_6_E() {BIT_SET(6,E);ft = 8;}
static void set_6_H() {BIT_SET(6,H);ft = 8;}
static void set_6_L() {BIT_SET(6,L);ft = 8;}
static void set_6_HL() {BIT_SET(6,MEM(HL));ft = 16;}

static void set_7_A() {BIT_SET(7,A);ft = 8;}
static void set_7_B() {BIT_SET(7,B);ft = 8;}
static void set_7_C() {BIT_SET(7,C);ft = 8;}
static void set_7_D() {BIT_SET(7,D);ft = 8;}
static void set_7_E() {BIT_SET(7,E);ft = 8;}
static void set_7_H() {BIT_SET(7,H);ft = 8;}
static void set_7_L() {BIT_SET(7,L);ft = 8;}
static void set_7_HL() {BIT_SET(7,MEM(HL));ft = 16;}

// RES b,r
// Reset bit b in register r
#define BIT_RES(b, r) do {\
    r &= ~(1<<b);\
} while(0)

static void res_0_A() {BIT_RES(0,A);ft = 8;}
static void res_0_B() {BIT_RES(0,B);ft = 8;}
static void res_0_C() {BIT_RES(0,C);ft = 8;}
static void res_0_D() {BIT_RES(0,D);ft = 8;}
static void res_0_E() {BIT_RES(0,E);ft = 8;}
static void res_0_H() {BIT_RES(0,H);ft = 8;}
static void res_0_L() {BIT_RES(0,L);ft = 8;}
static void res_0_HL() {BIT_RES(0,MEM(HL));ft = 16;}

static void res_1_A() {BIT_RES(1,A);ft = 8;}
static void res_1_B() {BIT_RES(1,B);ft = 8;}
static void res_1_C() {BIT_RES(1,C);ft = 8;}
static void res_1_D() {BIT_RES(1,D);ft = 8;}
static void res_1_E() {BIT_RES(1,E);ft = 8;}
static void res_1_H() {BIT_RES(1,H);ft = 8;}
static void res_1_L() {BIT_RES(1,L);ft = 8;}
static void res_1_HL() {BIT_RES(1,MEM(HL));ft = 16;}

static void res_2_A() {BIT_RES(2,A);ft = 8;}
static void res_2_B() {BIT_RES(2,B);ft = 8;}
static void res_2_C() {BIT_RES(2,C);ft = 8;}
static void res_2_D() {BIT_RES(2,D);ft = 8;}
static void res_2_E() {BIT_RES(2,E);ft = 8;}
static void res_2_H() {BIT_RES(2,H);ft = 8;}
static void res_2_L() {BIT_RES(2,L);ft = 8;}
static void res_2_HL() {BIT_RES(2,MEM(HL));ft = 16;}

static void res_3_A() {BIT_RES(3,A);ft = 8;}
static void res_3_B() {BIT_RES(3,B);ft = 8;}
static void res_3_C() {BIT_RES(3,C);ft = 8;}
static void res_3_D() {BIT_RES(3,D);ft = 8;}
static void res_3_E() {BIT_RES(3,E);ft = 8;}
static void res_3_H() {BIT_RES(3,H);ft = 8;}
static void res_3_L() {BIT_RES(3,L);ft = 8;}
static void res_3_HL() {BIT_RES(3,MEM(HL));ft = 16;}

static void res_4_A() {BIT_RES(4,A);ft = 8;}
static void res_4_B() {BIT_RES(4,B);ft = 8;}
static void res_4_C() {BIT_RES(4,C);ft = 8;}
static void res_4_D() {BIT_RES(4,D);ft = 8;}
static void res_4_E() {BIT_RES(4,E);ft = 8;}
static void res_4_H() {BIT_RES(4,H);ft = 8;}
static void res_4_L() {BIT_RES(4,L);ft = 8;}
static void res_4_HL() {BIT_RES(4,MEM(HL));ft = 16;}

static void res_5_A() {BIT_RES(5,A);ft = 8;}
static void res_5_B() {BIT_RES(5,B);ft = 8;}
static void res_5_C() {BIT_RES(5,C);ft = 8;}
static void res_5_D() {BIT_RES(5,D);ft = 8;}
static void res_5_E() {BIT_RES(5,E);ft = 8;}
static void res_5_H() {BIT_RES(5,H);ft = 8;}
static void res_5_L() {BIT_RES(5,L);ft = 8;}
static void res_5_HL() {BIT_RES(5,MEM(HL));ft = 16;}

static void res_6_A() {BIT_RES(6,A);ft = 8;}
static void res_6_B() {BIT_RES(6,B);ft = 8;}
static void res_6_C() {BIT_RES(6,C);ft = 8;}
static void res_6_D() {BIT_RES(6,D);ft = 8;}
static void res_6_E() {BIT_RES(6,E);ft = 8;}
static void res_6_H() {BIT_RES(6,H);ft = 8;}
static void res_6_L() {BIT_RES(6,L);ft = 8;}
static void res_6_HL() {BIT_RES(6,MEM(HL));ft = 16;}

static void res_7_A() {BIT_RES(7,A);ft = 8;}
static void res_7_B() {BIT_RES(7,B);ft = 8;}
static void res_7_C() {BIT_RES(7,C);ft = 8;}
static void res_7_D() {BIT_RES(7,D);ft = 8;}
static void res_7_E() {BIT_RES(7,E);ft = 8;}
static void res_7_H() {BIT_RES(7,H);ft = 8;}
static void res_7_L() {BIT_RES(7,L);ft = 8;}
static void res_7_HL() {BIT_RES(7,MEM(HL));ft = 16;}

// JP nn
// Jump to address nn; nn = two byte immediate value. (LS byte first.)
static void jp_d16() {PC=_d16;ft=16;}

// JP cc,nn
// Jump to address n if following condition is true:
//  cc = NZ, Jump if Z flag is reset.
//  cc = Z, Jump if Z flag is set.
//  cc = NC, Jump if C flag is reset.
//  cc = C, Jump if C flag is set.

static void jp_NZ() {
    ft = 12;
    if(F&0x80 == 0x00) {
        PC = _d16;
        ft += 4;
    } else {
        PC += 2;
    }
}
static void jp_Z() {
    ft = 12;
    if(F&0x80 == 0x80) {
        PC = _d16;
        ft += 4;
    } else {
        PC += 2;
    }
}
static void jp_NC() {
    ft = 12;
    if(F&0x10 == 0x00) {
        PC = _d16;
        ft += 4;
    } else {
        PC += 2;
    }
}
static void jp_C() {
    ft = 12;
    if(F&0x0 == 0x10) {
        PC = _d16;
        ft += 4;
    } else {
        PC += 2;
    }
}

// JP (HL)
// Jump to address contained in HL
static void jp_HL() {PC=HL;ft = 4;}

// JR n
// Add n to current address and jump to it; (n = one byte signed immediate value)
static void jr_d8() {
    ft = 12;
    signed char d = _d8;
    if(d<0) {
        d = ~d + 1;
        PC-=d;
    } else {
        PC+=d;
    }
}

// JR cc,n
static void jr_NZ() {
    ft = 8;
    signed char d = _d8;
    if(F&0x80 == 0x00) {
        if(d<0) {
            d = ~d + 1;
            PC-=d;
        } else {
            PC+=d;
        }
        ft += 4;
    }
}
static void jr_Z() {
    ft = 8;
    signed char d = _d8;
    if(F&0x80 == 0x80) {
        if(d<0) {
            d = ~d + 1;
            PC-=d;
        } else {
            PC+=d;
        }
        ft += 4;
    }
}
static void jr_NC() {
    ft = 8;
    signed char d = _d8;
    if(F&0x10 == 0x00) {
        if(d<0) {
            d = ~d + 1;
            PC-=d;
        } else {
            PC+=d;
        }
        ft += 4;
    }
}
static void jr_C() {
    ft = 8;
    signed char d = _d8;
    if(F&0x0 == 0x10) {
        if(d<0) {
            d = ~d + 1;
            PC-=d;
        } else {
            PC+=d;
        }
        ft += 4;
    }
}

// CALL nn
static void call_nn() {
    const unsigned short addr = _d16;
    // SP-=2;
    MEM(--SP)=(unsigned char)PC>>8;
    MEM(--SP)=(unsigned char)PC;
    PC = addr;
    ft = 24;
}

// CALL cc,nn
// Call address n if following condition is true:
// cc = NZ, Call if Z flag is reset.
// cc = Z, Call if Z flag is set.
// cc = NC, Call if C flag is reset.
// cc = C, Call if C flag is set.

static void call_NZ() {
    ft = 12;
    if(F&0x80 == 0x00) {
        call_nn();
    }
}
static void call_Z() {
    ft = 12;
    if(F&0x80 == 0x80) {
        call_nn();
    }
}
static void call_NC() {
    ft = 12;
    if(F&0x10 == 0x00) {
        call_nn();
    }
}
static void call_C() {
    ft = 12;
    if(F&0x0 == 0x10) {
        call_nn();
    }
}

// RST n
// Push present address onto stack.
// Jump to address $0000 + n.
// n = $00,$08,$10,$18,$20,$28,$30,$38
#define RST_(addr) do {\
    MEM(--SP)=(unsigned char)PC>>8;\
    MEM(--SP)=(unsigned char)PC;\
    PC = (addr);\
    ft = 16;\
} while(0)

static void rst_00h() {
    RST_(0x00);
}
static void rst_08h() {
    RST_(0x08);
}
static void rst_10h() {
    RST_(0x10);
}
static void rst_18h() {
    RST_(0x18);
}
static void rst_20h() {
    RST_(0x20);
}
static void rst_28h() {
    RST_(0x28);
}
static void rst_30h() {
    RST_(0x30);
}
static void rst_38h() {
    RST_(0x38);
}

// RET
// Pop two bytes from stack & jump to that address

static void ret() {
    PC = MEM(SP++) | MEM(SP++) << 8;
    ft = 16;
}

// RET cc
// Return if following condition is true:
// (...)
static void ret_NZ() {
    ft = 8;
    if(F&0x80 == 0x00) {
        ret();
        ft = 20;
    }
}
static void ret_Z() {
    ft = 8;
    if(F&0x80 == 0x80) {
        ret();
        ft = 20;
    }
}
static void ret_NC() {
    ft = 8;
    if(F&0x10 == 0x00) {
        ret();
        ft = 20;
    }
}
static void ret_C() {
    ft = 8;
    if(F&0x0 == 0x10) {
        ret();
        ft = 20;
    }
}


// RETI
// Pop two bytes from stack & jump to that address then enable interrupts
static void reti() {
    IME = 1;
    ret();
}


// CB Map
static void (*cb_map[])() = {
    // 0x
    rlc_B, rlc_C, rlc_D, rlc_E, rlc_H, rlc_L, rlc_HL, rlc_A, rrc_B, rrc_C, rrc_D, rrc_E, rrc_H, rrc_L, rrc_HL, rrca,
    // 1x
    rl_B, rl_C, rl_D, rl_E, rl_H, rl_L, rl_HL, rl_A, rr_B, rr_C, rr_D, rr_E, rr_H, rr_L, rr_HL, rr_A,
    // 2x
    sla_B, sla_C, sla_D, sla_E, sla_H, sla_L, sla_HL, sla_A, sra_B, sra_C, sra_D, sra_E, sra_H, sra_L, sra_HL, sra_A,
    // 3x
    swap_B, swap_C, swap_D, swap_E, swap_H, swap_L, swap_HL, swap_A, srl_B, srl_C, srl_D, srl_E, srl_H, srl_L, srl_HL, srl_A,
    // 4x ~ 7x
    bit_0_B, bit_0_C, bit_0_D, bit_0_E, bit_0_H, bit_0_L, bit_0_HL, bit_0_A, bit_1_B, bit_1_C, bit_1_D, bit_1_E, bit_1_H, bit_1_L, bit_1_HL, bit_1_A,
    bit_2_B, bit_2_C, bit_2_D, bit_2_E, bit_2_H, bit_2_L, bit_2_HL, bit_2_A, bit_3_B, bit_3_C, bit_3_D, bit_3_E, bit_3_H, bit_3_L, bit_3_HL, bit_3_A,
    bit_4_B, bit_4_C, bit_4_D, bit_4_E, bit_4_H, bit_4_L, bit_4_HL, bit_4_A, bit_5_B, bit_5_C, bit_5_D, bit_5_E, bit_5_H, bit_5_L, bit_5_HL, bit_5_A,
    bit_6_B, bit_6_C, bit_6_D, bit_6_E, bit_6_H, bit_6_L, bit_6_HL, bit_6_A, bit_7_B, bit_7_C, bit_7_D, bit_7_E, bit_7_H, bit_7_L, bit_7_HL, bit_7_A,
    // 8x ~ Bx
    res_0_B, res_0_C, res_0_D, res_0_E, res_0_H, res_0_L, res_0_HL, res_0_A, res_1_B, res_1_C, res_1_D, res_1_E, res_1_H, res_1_L, res_1_HL, res_1_A,
    res_2_B, res_2_C, res_2_D, res_2_E, res_2_H, res_2_L, res_2_HL, res_2_A, res_3_B, res_3_C, res_3_D, res_3_E, res_3_H, res_3_L, res_3_HL, res_3_A,
    res_4_B, res_4_C, res_4_D, res_4_E, res_4_H, res_4_L, res_4_HL, res_4_A, res_5_B, res_5_C, res_5_D, res_5_E, res_5_H, res_5_L, res_5_HL, res_5_A,
    res_6_B, res_6_C, res_6_D, res_6_E, res_6_H, res_6_L, res_6_HL, res_6_A, res_7_B, res_7_C, res_7_D, res_7_E, res_7_H, res_7_L, res_7_HL, res_7_A,
    // Cx ~ Fx
    set_0_B, set_0_C, set_0_D, set_0_E, set_0_H, set_0_L, set_0_HL, set_0_A, set_1_B, set_1_C, set_1_D, set_1_E, set_1_H, set_1_L, set_1_HL, set_1_A,
    set_2_B, set_2_C, set_2_D, set_2_E, set_2_H, set_2_L, set_2_HL, set_2_A, set_3_B, set_3_C, set_3_D, set_3_E, set_3_H, set_3_L, set_3_HL, set_3_A,
    set_4_B, set_4_C, set_4_D, set_4_E, set_4_H, set_4_L, set_4_HL, set_4_A, set_5_B, set_5_C, set_5_D, set_5_E, set_5_H, set_5_L, set_5_HL, set_5_A,
    set_6_B, set_6_C, set_6_D, set_6_E, set_6_H, set_6_L, set_6_HL, set_6_A, set_7_B, set_7_C, set_7_D, set_7_E, set_7_H, set_7_L, set_7_HL, set_7_A,
};

static void CB() {
    cb_map[MEM(PC++)]();
    ct += ft;
    ft = 0;
}


static void (*op_map[])() = {
    // 0x
    nop, ld_BC_d16, ld_BC_A, inc_BC, inc_B, dec_B, ld_B_d8, rlca, ld_a16_SP, add_HL_BC, ld_A_BC, dec_BC, inc_C, dec_C, ld_C_d8, rrca,
    // 1x
    stop, ld_DE_d16, ld_DE_A, inc_DE, inc_D, dec_D, ld_D_d8, rla, jr_d8, add_HL_DE, ld_A_DE, dec_DE, inc_E, dec_E, ld_E_d8, rra,
    // 2x
    jr_NZ, ld_HL_d16, ld_HLI_A, inc_HL, inc_H, dec_H, ld_H_d8, XX, jr_Z, add_HL_HL, ld_A_HLI, dec_HL, inc_L, dec_L, ld_L_d8, cpl,
    //3x
    jr_NC, ld_SP_d16, ld_HLD_A, inc_SP, inc_HL, dec_HL, ld_HL_d8, scf, jr_C, add_HL_SP, ld_A_HLD, dec_SP, inc_A, dec_A, ld_A_d8, ccf,
    // 4x
    ld_B_B, ld_B_C, ld_B_D, ld_B_E, ld_B_H, ld_B_L, ld_B_HL, ld_B_A, ld_C_B, ld_C_C, ld_C_D, ld_C_E, ld_C_H, ld_C_L, ld_C_HL, ld_C_A,
    // 5x
    ld_D_B, ld_D_C, ld_D_D, ld_D_E, ld_D_H, ld_D_L, ld_D_HL, ld_D_A, ld_E_B, ld_E_C, ld_E_D, ld_E_E, ld_E_H, ld_E_L, ld_E_HL, ld_E_A,
    // 6x
    ld_H_B, ld_H_C, ld_H_D, ld_H_E, ld_H_H, ld_H_L, ld_H_HL, ld_H_A, ld_L_B, ld_L_C, ld_L_D, ld_L_E, ld_L_H, ld_L_L, ld_L_HL, ld_L_A,
    // 7x
    ld_HL_B, ld_HL_C, ld_HL_D, ld_HL_E, ld_HL_H, ld_HL_L, halt, ld_HL_A, ld_A_B, ld_A_C, ld_A_D, ld_A_E, ld_A_H, ld_A_L, ld_A_HL, ld_A_A,
    // 8x
    add_A_B, add_A_C, add_A_D, add_A_E, add_A_H, add_A_L, adc_A_HL, add_A_A, adc_A_B, adc_A_C, adc_A_D, adc_A_E, adc_A_H, adc_A_L, adc_A_HL, adc_A_A,
    // 9x
    sub_B, sub_C, sub_D, sub_E, sub_H, sub_L, sub_HL, sub_A, sbc_A_B, sbc_A_C, sbc_A_D, sbc_A_E, sbc_A_H, sbc_A_L, sbc_A_HL, sbc_A_A,
    // Ax
    and_B, and_C, and_D, and_E, and_H, and_L, and_HL, and_A, xor_B, xor_C, xor_D, xor_E, xor_H, xor_L, xor_HL, xor_A,
    // Bx
    or_B, or_C, or_D, or_E, or_H, or_L, or_HL, or_A, cp_B, cp_C, cp_D, cp_E, cp_H, cp_L, cp_HL, cp_A,
    // Cx
    ret_NZ, pop_BC, jp_NZ, jp_d16, call_NZ, push_BC, add_A_d8, rst_00h, ret_Z, ret, jp_Z, CB, call_Z, call_nn, adc_A_d8, rst_08h,
    // Dx
    ret_NC, pop_DE, jp_NC, XX, call_NC, push_DE, sub_d8, rst_10h, ret_C, reti, jp_C, XX, call_C, XX, sbc_A_d8, rst_18h,
    // Ex
    ld_a8_A, pop_HL, ld_rC_A, XX, XX, push_HL, and_d8, rst_20h, add_SP_d8, jp_HL, ld_a16_A, XX, XX, XX, xor_d8, rst_28h,
    // Fx
    ld_A_d8, pop_AF, ld_A_rC, di, XX, push_AF, or_d8, rst_30h, ldhl_SP_d8, ld_SP_HL, ld_A_a16, ei, XX, XX, cp_d8, rst_38h
};


void cpu_exe() {
    op_map[MEM(PC++)]();
    ct += ft;
    ft = 0;
}

void cpu_reset() {
    ft = 0;
    ct = 0;
    F = 0x00;
    A = 0x00;
    B = 0x00;
    C = 0x00;
    D = 0x00;
    E = 0x00;
    H = 0x00;
    L = 0x00;
    SP = 0x0000;
    PC = 0x0000;
}
