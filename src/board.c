#include "mmu.h"
#include "cpu.h"
#include "ppu.h"

#define FRAME_CLOCKS    ((unsigned int)(20 + 43 + 51) * (144 + 10) * 4)

export void reset() {
    cpu_reset();
    ppu_reset();
}

export void frame() {
    ct = 0;
    while(ct < 1) {
        cpu_exe();
        ppu_step(ct);
    }
}

export float *getTexture() {
    return texture[0][0];
}

export void debug() {
    cpu_debug();
}

int main() {
    reset();
    return 0;
}