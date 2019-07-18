#include "mmu.h"

unsigned char _bios[0x100];
unsigned char _rom[VRAM_BASE-ROM_BASE];
unsigned char _vram[ERAM_BASE-VRAM_BASE];
unsigned char _eram[RAM_BASE-ERAM_BASE];
unsigned char _ram[OAM_RAM_BASE-RAM_BASE];
unsigned char _oam[IO_BASE-OAM_RAM_BASE];
unsigned char _io[HRAM_BASE-IO_BASE];
unsigned char _hram[TOP_ADDR-HRAM_BASE];


unsigned char *mmu(unsigned short addr) {
    switch (addr & 0xF000) {
        // 16kB ROM bank #0
        case 0x0000:
            // BIOS
            if (IO_Reg->BOOT & 1 == 0) {
                // Boot ROM is active and intercepts accesses to 0x0000-0x00FF
                return &_bios[addr];
            } else {
                // Boot ROM is disabled and 0x0000-0x00FF works normally.
                return &_rom[addr];
            }
        case 0x1000:
        case 0x2000:
        case 0x3000:
        // 16kB switchable ROM bank
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            return &_rom[addr];
        // 8kB Video RAM
        case 0x8000:
        case 0x9000:
            return &_vram[addr & 0x1FFF];
        // 8kB switchable RAM bank
        case 0xA000:
        case 0xB000:
            return &_eram[addr & 0x1FFF];
        // 8kB Internal RAM
        case 0xC000:
        case 0xD000:
            return &_ram[addr & 0x1FFF];
        // Echo of 8kB Internal RAM
        case 0xE000:
        // Sprite Attrib Memory (OAM)
        case 0xF000:
            // The addresses E000-FE00 appear to access the internal RAM the same as C000-DE00. [* Game BoyTM CPU Manual]
            switch (addr & 0x0F00) {
                default:
                    return &_ram[addr & 0x1FFF];
                case 0xE00: // OAM
                    return &_oam[addr & 0x1FFF];
                case 0xF00:
                    if (addr >= HRAM_BASE) {
                        // Internal RAM (HRAM)
                        return &_hram[addr & 0x7F];
                    } else {
                        // I/O ports
                        return &_io[addr & 0xFF];
                    }
            }
    }
}