/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * stress.h
 * VGA video output "stress" tests (really just some screens to test video
 * colour and sync timings).
 *****************************************************************************/

#pragma once
#ifndef __vga__palette_h__
#define __vga__palette_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Write a bunch of grids to the video memory to test the VGA colour DACs.
 * Some space is left at the bottom of the screen which can be used for text or
 * something.
 */
void
vga_stress_palette(void);

/**
 * Write some gridlines to vram, which might help with testing the stability of
 * VGA synchronisation timings as well as positioning the active region.
 *
 * The separation of grid lines can be configured using `xinc` and `yinc`, and
 * the colour with `col`.
 */
void
vga_stress_grid(uint16_t xinc, uint16_t yinc, uint8_t col);

/**
 * Write a font atlas to vram which might be helpful when diagnosing font
 * issues. This will include non-'printable' character codes (0-31) and the
 * space (32).
 *
 * The colour of the output atlas can be changed with `col`.
 */
void
vga_stress_atlas(uint8_t col);

#ifdef __cplusplus
}
#endif
#endif
