/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * lvldata.h
 * Handling level/world data (i.e. graphics primitives).
 *****************************************************************************/

#pragma once
#ifndef __level__lvldata_h__
#define __level__lvldata_h__
#ifdef __cplusplus
extern "C" {
#endif

#include "../renderer/geom.h"

// TODO temp camera data
extern const struct geom_cam lvl_test_cam;
// TODO temp world geometry data
extern const struct geom_polygon lvl_test_polys[GEOM_MAX_POLYS];

#ifdef __cplusplus
}
#endif
#endif
