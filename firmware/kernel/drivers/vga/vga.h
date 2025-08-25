/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * vga.h
 * Main driver interface for VGA video output via PIO and DMA.
 *****************************************************************************/

#pragma once
#ifndef __video__vga_h__
#define __video__vga_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define VGA_WIDTH       320 // the width of the used screen region
#define VGA_HEIGHT      200 // the height of the used screen region
#define VGA_SCAN_WIDTH  320 // actual width of the screen active region
#define VGA_SCAN_HEIGHT 400 // actual height of the screen active region

/**
 * VGA configuration fields.
 * GPIO field names relate directly to the respective pins on the schematic.
 */
struct vga_config {
    uint8_t gpio_b2g3r3_base; // base RGB (physically B2G3R3) GPIO
    uint8_t gpio_hsync;       // hsync pulse GPIO
    uint8_t gpio_vsync;       // vsync pulse GPIO
};

/**
 * Configure the VGA driver to drive video output on the pins specified in the
 * `op` struct.
 */
void
vga_init(const struct vga_config cfg);

#ifdef __cplusplus
}
#endif
#endif
