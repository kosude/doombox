/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * geom.h
 * Geometric struct definitions for game rendering.
 *****************************************************************************/

#pragma once
#ifndef __renderer__geom_h__
#define __renderer__geom_h__
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// TODO these can change per level
#define GEOM_MAX_POLYS 10
#define GEOM_MAX_VERTS 8

// geom_vec2 struct shorthand
#define GEOM_VEC2(x, y) \
    (struct geom_vec2)  \
    {                   \
        x, y            \
    }

/**
 * 2-D floating-point vector structure.
 */
struct geom_vec2 {
    float x;
    float y;
};

/**
 * A line segment, containing two vertices (start and end points).
 */
struct geom_lseg {
    struct geom_vec2 sta;
    struct geom_vec2 end;
};

/**
 * A polygon structure.
 */
struct geom_polygon {
    /** Array of vertices */
    struct geom_vec2 verts[GEOM_MAX_VERTS];
    /** Number of vertices in the verts array */
    size_t vertn;

    /** Height of the polygon */
    float hei;
    /** Distance between the polygon and the camera */
    float dist;
};

/**
 * A screen-space polygon - used to form the surface of the wall being
 * rendered to vram.
 */
struct geom_ss_polygon {
    /** Each sspoly represents a quadrilateral plane */
    struct geom_vec2 verts[4];

    /** Distance between the sspoly and the camera */
    float dist;
};

/**
 * Camera structure for storing player perspective data.
 */
struct geom_cam {
    /** Camera angle, in radians */
    float angle;

    /** Camera world-space position */
    struct geom_vec2 pos;
};

/**
 * Return the cross product magnitude of two 2-D vectors.
 */
inline float
geom_vec2_cross(const struct geom_vec2 a, const struct geom_vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

#ifdef __cplusplus
}
#endif
#endif
