#ifndef PPU_H
#define PPU_H

#define SCREEN_WIDTH    160 // Pixels
#define SCREEN_HEIGHT   144 // Pixels
#define TILE_SIZE       8   // Pixels
#define MAP_SIZE        32  // Tiles
#define SCREEN_TILES    20

// typedef unsigned short tileData_8[8];  // 8*8 tile
// typedef unsigned short tileData_16[16];  // 8*16 tile


extern unsigned char texture[144][160][3];
extern void ppu_reset();
extern void scanline();
extern void ppu_step(unsigned short clock);


#endif