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

/**
 * Write a bunch of grids to the video memory to test the VGA colour DACs.
 * Some space is left at the bottom of the screen which can be used for text or
 * something.
 */
void
vga_stress_palette(void);

#ifdef __cplusplus
}
#endif
#endif
