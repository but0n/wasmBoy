#include "ppu.h"
#include "mmu.h"

unsigned char texture[144][160][3] = {};


// RGB color in linear space
unsigned char colorLUT[4][3] = {
    {0, 0, 0},
    {0.3, 0.3, 0.3},
    {0.6, 0.6, 0.6},
    {1, 1, 1},
};

static unsigned short ppu_clock = 0;

void reset() {
    ppu_clock = 0;
    // Set up default palette data
    IO_Reg->BGP = 0b11100100;
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

void scanline() {
    // PPU is connect to VRAM directly
    unsigned short bgmap_offs = ((IO_Reg->LCDC & LCDC_BG_MAP) ? 0x9C00 : 0x9800) - VRAM_BASE;

    unsigned char liney = (IO_Reg->LY + IO_Reg->SCY) & 0xFF;
    unsigned char bgmapx = IO_Reg->SCX >> 3;

    bgmap_offs += liney >> 3 << 5; // Get line index of current Pixel Tiles (8x8 -> 32x32)

    // Decode tile data UV
    unsigned char u_start = IO_Reg->SCX & 7;
    unsigned char v = (IO_Reg->LY + IO_Reg->SCY) & 7;

    unsigned short tile_base = ((IO_Reg->LCDC & LCDC_TILE_SEL) ? 0x8000 : 0x8800) - VRAM_BASE;
    unsigned short (*tile_data)[8] = &_vram[tile_base]; // 256 x 8 x 2 Bytes


    unsigned char pixelCounter = 0;
    for(unsigned char i = 0; i < SCREEN_TILES+1; i++) {
        unsigned char ID = _vram[bgmap_offs + bgmapx + i]; // 256 tiles total, mask as 0xFF
        unsigned short tile = tile_data[ID][v];
        for(unsigned char p = 0; p < 8; p++) {
            // For each pixel (2 bits)
            unsigned char color_bit;
            unsigned char *color;
            color_bit = (tile >> ((7-p-u_start)<<1)) & 3 << 1;
            color = colorLUT[(IO_Reg->BGP >> color_bit) & 3];
            // Write color
            texture[liney][pixelCounter][0] = color[0];
            texture[liney][pixelCounter][1] = color[1];
            texture[liney][pixelCounter][2] = color[2];

            if(++pixelCounter > SCREEN_WIDTH) {
                return;
            }
        }
    }
}