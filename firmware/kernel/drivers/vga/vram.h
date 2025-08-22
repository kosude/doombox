/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * vram.h
 * Functions/symbols for reading and writing to the reserved video memory
 * (VRAM) block.
 *****************************************************************************/

#pragma once
#ifndef __vga__vram_h__
#define __vga__vram_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <hardware/pio.h>

extern uint8_t __vram_start[];

#define VRAM __vram_start
// #define VRAM_LEN 128000
#define VRAM_LEN 307200

/**
 * Configure and return a DMA channel for video data (VRAM) transfers, where
 * `sm` is the video signal state machine.
 */
uint32_t
vram_dma_channel_configure(pio_hw_t *const pio, uint8_t sm);

// TODO temp function
void
vram_init_palette();

#ifdef __cplusplus
}
#endif
#endif
