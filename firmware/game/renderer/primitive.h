/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * primitive.h
 * Functions for rendering primitive geometry to the screen.
 * Many of these algorithms are sourced from Yuriy Georgiev's tutorial at
 * https://yuriygeorgiev.com/2022/08/17/polygon-based-software-rendering-engine
 *****************************************************************************/

#pragma once
#ifndef __renderer__primitive_h__
#define __renderer__primitive_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Draw a line to the screen, connecting the two specified screen-space points
 * (x/y)0 and (x/y)1 (within the 320x200 viewport bounds), via digital
 * differential analyser (DDA) rasterisation.
 */
void
render_prim_line(const int32_t x0,
                 const int32_t y0,
                 const int32_t x1,
                 const int32_t y1,
                 const uint8_t col);

#ifdef __cplusplus
}
#endif
#endif
