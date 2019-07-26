#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "joypad.h"
#include <stdio.h>

#define FRAME_CLOCKS    ((unsigned int)(20 + 43 + 51) * (144 + 10) * 4)

export void reset() {
    cpu_reset();
    ppu_reset();
}

export void step() {
    ct = 0;
    cpu_exe();
    ppu_step(ct);
}

export void frame() {
    ct = 0;
    while(ct < FRAME_CLOCKS) {
        cpu_exe();
        ppu_step(ct);
    }
}

export unsigned char *getTexture() {
    return texture[0][0];
}

export unsigned char *getROM() {
    return _rom;
}
export unsigned char *getBIOS() {
    return _bios;
}

export void debug() {
    cpu_debug();
}

export void rom_info() {
    printf("Title: %s\n", ROM_Header->TITLE);
    unsigned char banks = 0;
    switch (ROM_Header->ROM_SIZE) {
        case 0x52:
            banks = 72;
            break;
        case 0x53:
            banks = 80;
            break;
        case 0x54:
            banks = 96;
            break;
        default:
            banks = 1 << ((ROM_Header->ROM_SIZE & 7) + 1);
            break;
    }
    printf("\t - ROM Size: %d banks\n", banks);
    printf("\t - Cartridge Type: %02X\n", ROM_Header->CART_TYPE);
    printf("\t - Color Flag: %02X\n", ROM_Header->COLOR_FLAG);
}


export void dir_up(unsigned char status) {toggle_dir_up(status);}
export void dir_down(unsigned char status) {toggle_dir_down(status);}
export void dir_left(unsigned char status) {toggle_dir_left(status);}
export void dir_right(unsigned char status) {toggle_dir_right(status);}
export void btn_select(unsigned char status) {toggle_btn_select(status);}
export void btn_start(unsigned char status) {toggle_btn_start(status);}
export void btn_B(unsigned char status) {toggle_btn_B(status);}
export void btn_A(unsigned char status) {toggle_btn_A(status);}

int main() {
    reset();
    return 0;
}