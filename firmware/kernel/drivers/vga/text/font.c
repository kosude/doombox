/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "font.h"
#include "../vga.h"
#include "../vram.h"

#include <kstdio.h>

// clang-format off
// ref: https://www.ascii-code.com/
// font bytes from https://gitlab.com/kosude/doom
uint64_t __vgafont[128] = {
    // control characters (0-31)
    0x0,                 /* NUL */
    0x0,                 /* SOH */
    0x0,                 /* STX */
    0x0,                 /* ETX */
    0x0,                 /* EOT */
    0x0,                 /* ENQ */
    0x0,                 /* ACK */
    0x0,                 /* BEL */
    0x0,                 /* BS  */
    0x0,                 /* HT  */
    0x0,                 /* LF  */
    0x0,                 /* VT  */
    0x0,                 /* FF  */
    0x0,                 /* CR  */
    0x0,                 /* SO  */
    0x0,                 /* SI  */
    0x0,                 /* DLE */
    0x0,                 /* DC1 */
    0x0,                 /* DC2 */
    0x0,                 /* DC3 */
    0x0,                 /* DC4 */
    0x0,                 /* NAK */
    0x0,                 /* SYN */
    0x0,                 /* ETB */
    0x0,                 /* CAN */
    0x0,                 /* EM  */
    0x0,                 /* SUB */
    0x0,                 /* ESC */
    0x0,                 /* FS  */
    0x0,                 /* GS  */
    0x0,                 /* RS  */
    0x0,                 /* US  */

    // printable characters (32-127)
    0x0,                 /* SP  */
    0x183C3C1818001800,  /* !   */
    0x3636000000000000,  /* "   */
    0x36367F367F363600,  /* #   */
    0x0C3E031E301F0C00,  /* $   */
    0x006333180C666300,  /* %   */
    0x1C361C6E3B336E00,  /* &   */
    0x0606030000000000,  /* '   */
    0x180C0606060C1800,  /* (   */
    0x060C1818180C0600,  /* )   */
    0x00663CFF3C660000,  /* *   */
    0x000C0C3F0C0C0000,  /* +   */
    0x00000000000C0C06,  /* ,   */
    0x0000003F00000000,  /* -   */
    0x00000000000C0C00,  /* .   */
    0x6030180C06030100,  /* /   */
    0x3E63737B6F673E00,  /* 0   */
    0x0C0E0C0C0C0C3F00,  /* 1   */
    0x1E33301C06333F00,  /* 2   */
    0x1E33301C30331E00,  /* 3   */
    0x383C36337F307800,  /* 4   */
    0x3F031F3030331E00,  /* 5   */
    0x1C06031F33331E00,  /* 6   */
    0x3F3330180C0C0C00,  /* 7   */
    0x1E33331E33331E00,  /* 8   */
    0x1E33333E30180E00,  /* 9   */
    0x000C0C00000C0C00,  /* :   */
    0x000C0C00000C0C06,  /* ;   */
    0x180C0603060C1800,  /* <   */
    0x00003F00003F0000,  /* =   */
    0x060C1830180C0600,  /* >   */
    0x1E3330180C000C00,  /* ?   */
    0x3E637B7B7B031E00,  /* @   */
    0x0C1E33333F333300,  /* A   */
    0x3F66663E66663F00,  /* B   */
    0x3C66030303663C00,  /* C   */
    0x1F36666666361F00,  /* D   */
    0x7F46161E16467F00,  /* E   */
    0x7F46161E16060F00,  /* F   */
    0x3C66030373667C00,  /* G   */
    0x3333333F33333300,  /* H   */
    0x1E0C0C0C0C0C1E00,  /* I   */
    0x7830303033331E00,  /* J   */
    0x6766361E36666700,  /* K   */
    0x0F06060646667F00,  /* L   */
    0x63777F7F6B636300,  /* M   */
    0x63676F7B73636300,  /* N   */
    0x1C36636363361C00,  /* O   */
    0x3F66663E06060F00,  /* P   */
    0x1E3333333B1E3800,  /* Q   */
    0x3F66663E36666700,  /* R   */
    0x1E33070E38331E00,  /* S   */
    0x3F2D0C0C0C0C1E00,  /* T   */
    0x3333333333333F00,  /* U   */
    0x33333333331E0C00,  /* V   */
    0x6363636B7F776300,  /* W   */
    0x6363361C1C366300,  /* X   */
    0x3333331E0C0C1E00,  /* Y   */
    0x7F6331184C667F00,  /* Z   */
    0x1E06060606061E00,  /* [   */
    0x03060C1830604000,  /* \   */
    0x1E18181818181E00,  /* ]   */
    0x081C366300000000,  /* ^   */
    0x00000000000000FF,  /* _   */
    0x0C0C180000000000,  /* `   */
    0x00001E303E336E00,  /* a   */
    0x0706063E66663B00,  /* b   */
    0x00001E3303331E00,  /* c   */
    0x3830303e33336E00,  /* d   */
    0x00001E333f031E00,  /* e   */
    0x1C36060f06060F00,  /* f   */
    0x00006E33333E301F,  /* g   */
    0x0706366E66666700,  /* h   */
    0x0C000E0C0C0C1E00,  /* i   */
    0x300030303033331E,  /* j   */
    0x070666361E366700,  /* k   */
    0x0E0C0C0C0C0C1E00,  /* l   */
    0x0000337F7F6B6300,  /* m   */
    0x00001F3333333300,  /* n   */
    0x00001E3333331E00,  /* o   */
    0x00003B66663E060F,  /* p   */
    0x00006E33333E3078,  /* q   */
    0x00003B6E66060F00,  /* r   */
    0x00003E031E301F00,  /* s   */
    0x080C3E0C0C2C1800,  /* t   */
    0x0000333333336E00,  /* u   */
    0x00003333331E0C00,  /* v   */
    0x0000636B7F7F3600,  /* w   */
    0x000063361C366300,  /* x   */
    0x00003333333E301F,  /* y   */
    0x00003F190C263F00,  /* z   */
    0x380C0C070C0C3800,  /* {   */
    0x1818180018181800,  /* |   */
    0x070C0C380C0C0700,  /* }   */
    0x6E3B000000000000,  /* ~   */
    0x0                  /* DEL */
};
// clang-format on

void
vga_fontat(const char ch, const uint8_t col, const uint16_t x, const uint16_t y)
{
    // start at the last byte in the glyph; decrement for the right way up
    uint8_t *glyptr = (uint8_t *)(&__vgafont[ch]) + 7;

    // iterate over the positioned 8x8 square in vram
    size_t vofs;
    for (uint8_t i = 0; i < 8; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            // TODO update for double-buffering
            vofs = ((y + i) * VGA_WIDTH) + (x + j); // index/offset into vram

            if (*glyptr & (1 << j)) {
                VRAM[vofs] = col;
            }
        }

        // decrement glyph byte (i.e. advance to the next row)
        glyptr--;
    }
}
