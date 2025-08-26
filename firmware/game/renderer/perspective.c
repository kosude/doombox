/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <math.h>

#include "kernel/drivers/vga/vga.h"

#include "perspective.h"
#include "primitive.h"

// width and height ratios
#define persp_wratio (VGA_WIDTH * 0.5)
#define persp_hratio ((VGA_WIDTH * VGA_HEIGHT) / 60.0)
// center of each viewport dimension
#define persp_wcenter (VGA_WIDTH * 0.5)
#define persp_hcenter (VGA_HEIGHT * 0.5)

// Line intersection function, courtesy of:
// https://yuriygeorgiev.com/2022/08/17/polygon-based-software-rendering-engine
static struct geom_vec2
_line_intersect(const struct geom_vec2 a,
                const struct geom_vec2 b,
                const struct geom_vec2 c,
                const struct geom_vec2 d)
{
    struct geom_vec2 ret;
    ret.x = geom_vec2_cross(a, b);
    ret.y = geom_vec2_cross(c, d);

    const float detr = 1
                       / geom_vec2_cross(GEOM_VEC2(a.x - b.x, a.y - b.y),
                                         GEOM_VEC2(c.x - d.x, c.y - d.y));

    ret.x = geom_vec2_cross(GEOM_VEC2(ret.x, a.x - b.x),
                            GEOM_VEC2(ret.y, c.x - d.x))
            * detr;
    ret.y = geom_vec2_cross(GEOM_VEC2(ret.x, a.y - b.y),
                            GEOM_VEC2(ret.y, c.y - d.y))
            * detr;

    return ret;
}

// Front-face checking for culling back-faces
static int
_front_face(const struct geom_vec2 cam,
            const struct geom_vec2 a,
            const struct geom_vec2 b)
{
    const int cross = geom_vec2_cross(GEOM_VEC2(a.x - cam.x, a.y - cam.y),
                                      GEOM_VEC2(b.x - cam.x, b.y - cam.y));
    if (!cross) {
        return 0;
    }
    return (cross > 0) ? 1 : -1;
}

void
render_perspective(const struct geom_polygon *const polys,
                   const size_t polyn,
                   const struct geom_cam *const cam)
{
    const struct geom_vec2 view = cam->pos;
    const float angle = cam->angle;

    // iterate through each polygon
    for (size_t p = 0; p < polyn; p++) {
        const float height = -polys[p].hei;

        // iterate through each vertex in the polygon
        for (size_t i = 0; i < polys[p].vertn - 1; i++) {
            // adjacent vertices
            const struct geom_vec2 p1 = polys[p].verts[i],
                                   p2 = polys[p].verts[i + 1];

            // backface culling
            if (_front_face(view, p1, p2) > 0) {
                continue;
            }

            // get distances of each vertex from the camera
            float distx1 = p1.x - view.x, disty1 = p1.y - view.y,
                  distx2 = p2.x - view.x, disty2 = p2.y - view.y;

            // compute pseudo Z coordinates of each vertex
            float z1 = (distx1 * cos(angle)) + (disty1 * sin(angle)),
                  z2 = (distx2 * cos(angle)) + (disty2 * sin(angle));

            // recalculate X distances based on angle from the camera
            distx1 = (distx1 * sin(angle)) - (disty1 * cos(angle));
            distx2 = (distx2 * sin(angle)) - (disty2 * cos(angle));

            // Z-clipping...
            // FIXME: performs AWFULLY (~90 fps down to ~2 fps somehow)
            //        needs some serious optimisations
            if (z1 > 0 || z2 > 0) {
                struct geom_vec2 i1 =
                    _line_intersect(GEOM_VEC2(distx1, z1),
                                    GEOM_VEC2(distx2, z2),
                                    GEOM_VEC2(-0.0001, 0.0001),
                                    GEOM_VEC2(-20.0, 5.0));
                struct geom_vec2 i2 =
                    _line_intersect(GEOM_VEC2(distx1, z1),
                                    GEOM_VEC2(distx2, z2),
                                    GEOM_VEC2(0.0001, 0.0001),
                                    GEOM_VEC2(20.0, 5.0));

                if (z1 <= 0) {
                    if (i1.y > 0) {
                        distx1 = i1.x;
                        z1 = i1.y;
                    } else {
                        distx1 = i2.x;
                        z1 = i2.y;
                    }
                }
                if (z2 <= 0) {
                    if (i1.y > 0) {
                        distx2 = i1.x;
                        z2 = i1.y;
                    } else {
                        distx2 = i2.x;
                        z2 = i2.y;
                    }
                }
            } else {
                // skip to next vertex
                continue;
            }

            const float x1 = -distx1 * (persp_wratio / z1);
            const float x2 = -distx2 * (persp_wratio / z2);
            const float y1a = (height - persp_hratio) / z1;
            const float y1b = persp_hratio / z1;
            const float y2a = (height - persp_hratio) / z2;
            const float y2b = persp_hratio / z2;

            // TODO
            const uint8_t col = 0x07;

            // draw the quadrilateral made up of the current and next vertices
            render_prim_line(x1 + persp_wcenter,
                             y1a + persp_hcenter,
                             x2 + persp_wcenter,
                             y2a + persp_hcenter,
                             col);
            render_prim_line(x1 + persp_wcenter,
                             y1b + persp_hcenter,
                             x2 + persp_wcenter,
                             y2b + persp_hcenter,
                             col);
            render_prim_line(x1 + persp_wcenter,
                             y1a + persp_hcenter,
                             x1 + persp_wcenter,
                             y1b + persp_hcenter,
                             col);
            render_prim_line(x2 + persp_wcenter,
                             y2a + persp_hcenter,
                             x2 + persp_wcenter,
                             y2b + persp_hcenter,
                             col);
        }
    }
}
