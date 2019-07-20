#include "mmu.h"
#include "cpu.h"
#include "ppu.h"

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

int main() {
    reset();
    return 0;
}