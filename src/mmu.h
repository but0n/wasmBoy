#ifndef MMU_H
#define MMU_H


// Memory map
#define ROM_BASE            ((unsigned short)0x0000) /*!< 32kB Cartrigbe ROM base address */
#define VRAM_BASE           ((unsigned short)0x8000) /*!< 8kB Video RAM base address */
#define ERAM_BASE           ((unsigned short)0xA000) /*!< External RAM (8kB switchable RAM bank) base address */
#define RAM_BASE            ((unsigned short)0xC000) /*!< 8kB Internal RAM base address */
#define OAM_RAM_BASE        ((unsigned short)0xFE00) /*!< Sprite Attrib Memory (OAM) base address, OAM is 160 bytes, remaining bytes read as 0 */
#define IO_BASE             ((unsigned short)0xFF00) /*!< I/O ports base address */
#define HRAM_BASE           ((unsigned short)0xFF80) /*!< HRAM base address */
#define TOP_ADDR            ((unsigned short)0xFFFF)

#define MEM(addr) (*mmu(addr))

typedef struct {
    unsigned char P1;
    unsigned char SB;
    unsigned char SC;
    unsigned char x03;
    unsigned char DIV;
    unsigned char TIMA;
    unsigned char TMA;
    unsigned char TAC;
    unsigned char x08[7];  // 0x08 ~ 0x0E
    unsigned char NR10;
    unsigned char NR11;
    unsigned char NR12;
    unsigned char NR13;
    unsigned char NR14;
    unsigned char x15;
    unsigned char NR21;
    unsigned char NR22;
    unsigned char NR23;
    unsigned char NR24;
    unsigned char NR30;
    unsigned char NR31;
    unsigned char NR32;
    unsigned char NR33;
    unsigned char NR34;
    unsigned char x1F;
    unsigned char NR41;
    unsigned char NR42;
    unsigned char NR43;
    unsigned char NR44;
    unsigned char NR50;
    unsigned char NR51;
    unsigned char NR52;
    unsigned char x27[9];   // 0x27 ~ 0x2F
    unsigned char WAV[16];
    unsigned char LCDC;
    unsigned char STAT;
    unsigned char SCY;
    unsigned char SCX;
    unsigned char LX;
    unsigned char LYC;
    unsigned char DMA;
    unsigned char BGP;
    unsigned char OBP0;
    unsigned char OBP1;
    unsigned char WY;
    unsigned char WX;
    unsigned char x4C;
    unsigned char KEY1;
    unsigned char x4E;
    unsigned char VBK;
    unsigned char BOOT;
    unsigned char HDMA1;
    unsigned char HDMA2;
    unsigned char HDMA3;
    unsigned char HDMA4;
    unsigned char HDMA5;
    unsigned char RP;
    unsigned char x57[17];
    unsigned char BCPS;
    unsigned char BCPD;
    unsigned char OCPS;
    unsigned char OCPD;
    unsigned char x6C[4];   // 0x6C ~ 0x6F
    unsigned char SVBK;
    unsigned char x71[5];   // 0x71 ~ 0x75
    unsigned char PCM12;
    unsigned char PCM34;
    unsigned char x78[86];  // 0x78 ~ 0xFE
    unsigned char IE;
} REG_TypeDef;


extern unsigned char *mmu(unsigned short addr);

#endif