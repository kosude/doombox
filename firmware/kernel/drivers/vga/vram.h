/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * vram.h
 * Functions/symbols for reading and writing to the reserved video memory
 * (VRAM) block, and bridging it to the video signal PIO via DMA.
 *****************************************************************************/

#pragma once
#ifndef __vga__vram_h__
#define __vga__vram_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <hardware/pio.h>

// VRAM memory block, containing space for both framebuffers
extern uint8_t __vram_start[];
// Pointer to the VRAM back buffer
extern uint8_t *__vram_back;

#define VRAM        __vram_back // Vram back buffer for pixels to be updated
#define VRAM_LEN    128000      // 2 * fb width * fb height
#define VRAM_FB_LEN 64000       // fb width * fb height

/**
 * Configure and return a DMA channel for video data (VRAM) transfers, where
 * `sm` is the video signal state machine.
 */
uint32_t
vram_dma_channel_configure(pio_hw_t *const pio, uint8_t sm);

/**
 * Swap the front and back buffer regions in vram.
 *
 * The word 'swap' isn't strictly true, as the contents of the back buffer are
 * just copied to the front buffer.
 */
void
vram_swap(void);

/**
 * Clear the entire video memory block to a given value.
 */
void
vram_clear(const uint8_t col);

#ifdef __cplusplus
}
#endif
#endif
