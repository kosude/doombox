/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * vga.h
 * Driver for VGA video output through SPI.
 *****************************************************************************/

#pragma once
#ifndef __video__vga_h__
#define __video__vga_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * VGA configuration fields.
 * GPIO field names relate directly to the respective pins on the schematic.
 */
struct vga_config {
    // PIO instance to reserve for VGA state machines (0 or 1)
    uint8_t pio;

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
