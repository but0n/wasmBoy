#include "joypad.h"
#include "mmu.h"

unsigned char joypad_dir = 0xFF;    // direction
unsigned char joypad_btn = 0xFF;    // button

void toggle_dir_up(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_dir &= ~P1_IN_P12_USL;
    } else {
        joypad_dir |= P1_IN_P12_USL;
    }
}

void toggle_dir_down(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_dir &= ~P1_IN_P13_DST;
    } else {
        joypad_dir |= P1_IN_P13_DST;
    }
}

void toggle_dir_left(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_dir &= ~P1_IN_P11_LB;
    } else {
        joypad_dir |= P1_IN_P11_LB;
    }
}

void toggle_dir_right(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_dir &= ~P1_IN_P10_RA;
    } else {
        joypad_dir |= P1_IN_P10_RA;
    }
}

void toggle_btn_select(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_btn &= ~P1_IN_P12_USL;
    } else {
        joypad_btn |= P1_IN_P12_USL;
    }
}

void toggle_btn_start(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_btn &= ~P1_IN_P13_DST;
    } else {
        joypad_btn |= P1_IN_P13_DST;
    }
}

void toggle_btn_B(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_btn &= ~P1_IN_P11_LB;
    } else {
        joypad_btn |= P1_IN_P11_LB;
    }
}

void toggle_btn_A(unsigned char status) {
    IO_Reg->IF |= IE_JOYPAD;
    if (status) {
        joypad_btn |= P1_IN_P10_RA;
    } else {
        joypad_btn &= ~P1_IN_P10_RA;
    }
}

