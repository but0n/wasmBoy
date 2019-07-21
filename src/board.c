#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
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

int main() {
    reset();
    return 0;
}