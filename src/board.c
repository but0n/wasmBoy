#include "mmu.h"
#include "cpu.h"
#include "ppu.h"

#define FRAME_CLOCKS    ((unsigned int)(20 + 43 + 51) * (144 + 10) * 4)

void reset() {
    cpu_reset();
    // ppu_reset();
}

export void frame() {
    ct = 0;
    while(ct < FRAME_CLOCKS) {
        cpu_exe();
        ppu_step(ct);
    }
}

export unsigned char *getTexture() {
    return texture;
}

// export void debug() {
//     cpu_debug();
// }

void main() {
    reset();
    // cpu_debug();
}