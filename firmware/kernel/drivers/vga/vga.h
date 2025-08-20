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
 * VGA output line configuration.
 * GPIO field names relate directly to the respective pins on the schematic.
 */
struct vga_outputs {
    uint8_t gpio_rgb8;
    uint8_t gpio_hsync;
    uint8_t gpio_vsync;
};

/**
 * Configure the VGA driver to drive video output on the pins specified in the
 * `op` struct.
 */
void
vga_init(const struct vga_outputs op);

#ifdef __cplusplus
}
#endif
#endif
