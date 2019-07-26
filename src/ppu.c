#include "ppu.h"
#include "mmu.h"
#include "cpu.h"

unsigned char texture[144][160][3] = {};


// RGB color in linear space
unsigned char colorLUT[4][3] = {
    {255, 255, 255},
    {88, 88, 88},
    {160, 160, 160},
    {8, 8, 8},
};

static unsigned short ppu_clock = 0;

void ppu_reset() {
    ppu_clock = 0;
    // Set up default palette data
    IO_Reg->BGP = 0b11100100;
    IO_Reg->OBP0 = 0b11100100;
    IO_Reg->OBP1 = 0b11100100;

    IO_Reg->NR52 = 0xF1;
    IO_Reg->LCDC = 0x91;
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

void ppu_step(unsigned short clock) {

    if (isStop)
        return;

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
                scanline();
                // Update Interrupt Flag
                if (IO_Reg->STAT & STAT_INTR_M0) {
                    IO_Reg->IF |= IE_STAT;
                }
            }
            break;

        // H-Blank 51 clocks* (1MHz)
        case LCD_MODE_HBLANK:
            if (ppu_clock >= 204) {
                ppu_clock = 0;
                // Increase LY register
                IO_Reg->LY++;
                if (IO_Reg->LY == IO_Reg->LYC) {
                    IO_Reg->STAT |= STAT_LYC_STAT;
                    // Update Interrupt Flag
                    if (IO_Reg->STAT & STAT_INTR_LYC) {
                        IO_Reg->IF |= IE_STAT;
                    }
                } else {
                    IO_Reg->STAT &= ~STAT_LYC_STAT;
                }
                // The LY indicates the vertical line to which
                //  the present data is transferred to the LCD
                //  Driver. The LY can take on any value
                //  between 0 through 153. The values between
                //  144 and 153 indicate the V-Blank period.
                //  Writing will reset the counter.

                if (IO_Reg->LY == 144) {
                    IO_Reg->STAT &= ~STAT_LCD_MODE;
                    IO_Reg->STAT |= LCD_MODE_VBLANK;
                    IO_Reg->IF |= IE_VBLANK;
                    // Update Interrupt Flag
                    if (IO_Reg->STAT & STAT_INTR_M1) {
                        IO_Reg->IF |= IE_STAT;
                    }
                } else {
                    IO_Reg->STAT &= ~STAT_LCD_MODE;
                    IO_Reg->STAT |= LCD_MODE_OAM;
                    // Update Interrupt Flag
                    if (IO_Reg->STAT & STAT_INTR_M2) {
                        IO_Reg->IF |= IE_STAT;
                    }
                }
            }
            break;

        // V-Blank 10 lines
        case LCD_MODE_VBLANK:
            if (ppu_clock >= 456) {
                ppu_clock = 0;
                // Increase LY register
                IO_Reg->LY++;
                if (IO_Reg->LY == IO_Reg->LYC) {
                    IO_Reg->STAT |= STAT_LYC_STAT;
                    // Update Interrupt Flag
                    if (IO_Reg->STAT & STAT_INTR_LYC) {
                        IO_Reg->IF |= IE_STAT;
                    }
                } else {
                    IO_Reg->STAT &= ~STAT_LYC_STAT;
                }

                if (IO_Reg->LY > 153) {
                    // Restart
                    IO_Reg->LY = 0;
                    IO_Reg->STAT &= ~STAT_LCD_MODE;
                    IO_Reg->STAT |= LCD_MODE_OAM;
                    // Update Interrupt Flag
                    if (IO_Reg->STAT & STAT_INTR_M2) {
                        IO_Reg->IF |= IE_STAT;
                    }
                }
            }
            break;
    }
}

void drawtile(unsigned short tile) {

}

void scanline() {

    unsigned char liney = (IO_Reg->LY + IO_Reg->SCY) & 0xFF;

    // Decode tile data UV
    unsigned char u_start = IO_Reg->SCX & 7;
    unsigned char v = (IO_Reg->LY + IO_Reg->SCY) & 7;

    // BG & Window Tile Data Select
    unsigned short tile_base = ((IO_Reg->LCDC & LCDC_TILE_SEL) ? 0x8000 : 0x8800) - VRAM_BASE;
    unsigned char (*tile_data)[8][2] = (unsigned char (*)[8][2])&_vram[tile_base]; // 256 x 8 x 2 Bytes

    // BG Tile Map Display Select
    unsigned short bgmap_offs = ((IO_Reg->LCDC & LCDC_BG_MAP) ? 0x9C00 : 0x9800) - VRAM_BASE;
    unsigned char (*bgmap)[32] = (unsigned char (*)[32])&_vram[bgmap_offs];
    unsigned char bgmapx = IO_Reg->SCX >> 3;
    unsigned char bgmapy = liney >> 3 & 0x1F;

    // Window Tile Map Display Select
    unsigned short winmap_offs = ((IO_Reg->LCDC & LCDC_WIN_MAP) ? 0x9C00 : 0x9800) - VRAM_BASE;
    unsigned char (*winmap)[32] = (unsigned char (*)[32])&_vram[winmap_offs];
    unsigned char winmapx = IO_Reg->WX >> 3;
    unsigned char winmapy = (IO_Reg->LY - IO_Reg->WY) >> 3 & 0x1F;
    unsigned char wintiley = (IO_Reg->LY - IO_Reg->WY) & 7;

    unsigned char pixelCounter = 0;
    for(unsigned char i = 0; i < SCREEN_TILES+1; i++) {
        unsigned char ID = bgmap[bgmapy][(bgmapx + i) & 0x1F]; // 256 tiles total, mask as 0xFF
        unsigned char win_tile_ID = winmap[winmapy][(winmapx + i) & 0x1F];
        unsigned char *tile = tile_data[ID][v];
        unsigned char *win_tile = tile_data[win_tile_ID][v];
        for(unsigned char p = 0; p < 8; p++) {
            // For each pixel (2 bits)
            unsigned char color_bit;
            // Draw Window
            if ((IO_Reg->LCDC & LCDC_WIN_EN) && (IO_Reg->LY >= IO_Reg->WY) && (pixelCounter >= IO_Reg->WX)) {
                color_bit = ((win_tile[0] >> (7 - p) << 1) & 2) | ((win_tile[1] >> (7 - p)) & 1);
            } else {
                if(u_start) {
                    u_start--;
                    continue;
                }
                color_bit = ((tile[0] >> (7 - p) << 1) & 2) | ((tile[1] >> (7 - p)) & 1);
            }
            unsigned char *color = colorLUT[(IO_Reg->BGP >> (color_bit << 1)) & 3];
            // Write color
            texture[liney][pixelCounter][0] = color[0];
            texture[liney][pixelCounter][1] = color[1];
            texture[liney][pixelCounter][2] = color[2];
            if(++pixelCounter >= SCREEN_WIDTH) {
                return;
            }
        }
    }
}