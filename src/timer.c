#include "timer.h"
#include "mmu.h"


static unsigned short timer_clock = 0;
static unsigned short timer_divclock = 0;

static unsigned short input_clock[] = {
    1024,
    16,
    64,
    256
};

void timer_step(unsigned char dt) {
    timer_clock += dt;
    timer_divclock += dt;

    if (timer_clock > 256) {
        timer_divclock = 0;
        IO_Reg->DIV++;
    }
    if (IO_Reg->TAC & TAC_EN) {
        if (timer_clock > input_clock[IO_Reg->TAC & TAC_CLK]) {
            timer_clock = 0;
            if (++IO_Reg->TIMA == 0) {
                IO_Reg->TIMA = IO_Reg->TMA;
                IO_Reg->IF |= IE_TIMER;
            }
        }
    }
}
