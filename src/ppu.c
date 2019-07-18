#include "ppu.h"
#include "mmu.h"

static unsigned short ppu_clock = 0;

void reset() {
    ppu_clock = 0;
}

// NOTE: PPU mode
// Period                       GPU mode number     *Time spent (clocks) (Ideal)
// Scanline (accessing OAM)     2                   80
// Scanline (accessing VRAM)	3                   172
// Horizontal blank             0                   204
// One line (scan and blank)	                    456
// Vertical blank               1                   4560 (10 lines)
// Full frame (scans and vblank)                    70224

// 0xFF41 STAT > NOTUSE : INTR_LYC : INTR_M2 : INTR_M1 : INTR_M0 : LYC_STAT : LCD_MODE<1:0>

void step(unsigned short clock) {

    ppu_clock += clock;

    switch (IO_Reg->STAT & STAT_LCD_MODE) {

        // OAM Search 20 clocks (1MHz)
        case LCD_MODE_OAM:
            if (ppu_clock >= 80) {
                ppu_clock = 0;
                IO_Reg->STAT &= ~STAT_LCD_MODE;
                IO_Reg->STAT |= LCD_MODE_TRANS;
            }
            break;

        // Pixel Transfer 43 clocks* (1MHz)
        case LCD_MODE_TRANS:
            if (ppu_clock >= 172) {
                ppu_clock = 0;
                IO_Reg->STAT &= ~STAT_LCD_MODE;
                IO_Reg->STAT |= LCD_MODE_HBLANK;
                // TODO: transfer scanline
            }
            break;

        // H-Blank 51 clocks* (1MHz)
        case LCD_MODE_HBLANK:
            if (ppu_clock >= 204) {
                ppu_clock = 0;
                // Increase LY register
                IO_Reg->LY++;
                // The LY indicates the vertical line to which
                //  the present data is transferred to the LCD
                //  Driver. The LY can take on any value
                //  between 0 through 153. The values between
                //  144 and 153 indicate the V-Blank period.
                //  Writing will reset the counter.

                if (IO_Reg->LY == 143) {
                    IO_Reg->STAT &= ~STAT_LCD_MODE;
                    IO_Reg->STAT |= LCD_MODE_VBLANK;
                    // TODO: Draw image
                } else {
                    IO_Reg->STAT &= ~STAT_LCD_MODE;
                    IO_Reg->STAT |= LCD_MODE_OAM;
                }
            }
            break;

        // V-Blank 10 lines
        case LCD_MODE_VBLANK:
            if (ppu_clock >= 456) {
                ppu_clock = 0;
                // Increase LY register
                IO_Reg->LY++;

                if (IO_Reg->LY > 153) {
                    // Restart
                    IO_Reg->STAT &= ~STAT_LCD_MODE;
                    IO_Reg->STAT |= LCD_MODE_OAM;
                }
            }
            break;
    }
}