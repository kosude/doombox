/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * perspective.h
 * Functions for rendering primitives to the screen through a perspective
 * projection as defined in a geom_cam datastructure.
 *****************************************************************************/

#pragma once
#ifndef __renderer__perspective_h__
#define __renderer__perspective_h__
#ifdef __cplusplus
extern "C" {
#endif

#include "geom.h"

/**
 * Draw an array of polygons (i.e. a level) with a perspective projection from
 * the camera's point of view.
 *
 * `polys` is an array of world-space polygons of length `polyn`, and `cam` is
 * the camera to view perspective from.
 */
void
render_perspective(const struct geom_polygon *const polys,
                   const size_t polyn,
                   const struct geom_cam *const cam);

#ifdef __cplusplus
}
#endif
#endif
