/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include "lvldata.h"

// clang-format off

const struct geom_cam lvl_test_cam = {
    .angle = 0.42,
    .pos = GEOM_VEC2(451.96, 209.24)
};

const struct geom_polygon lvl_test_polys[GEOM_MAX_POLYS] = {
    // first polygon (pentagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(141.00, 84.00 ),
            GEOM_VEC2(496.00, 81.00 ),
            GEOM_VEC2(553.00, 136.00),
            GEOM_VEC2(135.00, 132.00),
            GEOM_VEC2(141.00, 84.00 ),
        },
        .vertn = 5,
        .hei = 50000.00
    },
    // second polygon (pentagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(133.00, 441.00),
            GEOM_VEC2(576.00, 438.00),
            GEOM_VEC2(519.00, 493.00),
            GEOM_VEC2(132.00, 497.00),
            GEOM_VEC2(133.00, 441.00),
        },
        .vertn = 5,
        .hei = 50000.00
    },
    // third polygon (septagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(691.00, 165.00),
            GEOM_VEC2(736.00, 183.00),
            GEOM_VEC2(737.00, 229.00),
            GEOM_VEC2(697.00, 247.00),
            GEOM_VEC2(656.00, 222.00),
            GEOM_VEC2(653.00, 183.00),
            GEOM_VEC2(691.00, 165.00),
        },
        .vertn = 7,
        .hei = 10000.00
    },
    // fourth polygon (septagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(698.00, 330.00),
            GEOM_VEC2(741.00, 350.00),
            GEOM_VEC2(740.00, 392.00),
            GEOM_VEC2(699.00, 414.00),
            GEOM_VEC2(654.00, 384.00),
            GEOM_VEC2(652.00, 348.00),
            GEOM_VEC2(698.00, 330.00),
        },
        .vertn = 7,
        .hei = 10000.00
    },
    // fifth polygon (sexagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(419.00, 411.00),
            GEOM_VEC2(461.00, 311.00),
            GEOM_VEC2(404.00, 397.00),
            GEOM_VEC2(346.00, 395.00),
            GEOM_VEC2(348.00, 337.00),
            GEOM_VEC2(419.00, 311.00),
        },
        .vertn = 6,
        .hei = 50000.00
    },
    // sixth polygon (pentagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(897.00, 98.00),
            GEOM_VEC2(1079.00, 294.00),
            GEOM_VEC2(1028.00, 297.00),
            GEOM_VEC2(851.00, 96.00),
            GEOM_VEC2(897.00, 98.00),
        },
        .vertn = 5,
        .hei = 10000.00
    },
    // seventh polygon (pentagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(1025.00, 294.00),
            GEOM_VEC2(1080.00, 292.00),
            GEOM_VEC2(1149.00, 485.00),
            GEOM_VEC2(1072.00, 485.00),
            GEOM_VEC2(1024.00, 294.00),
        },
        .vertn = 5,
        .hei = 1000.00
    },
    // eighth polygon (pentagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(1070.00, 483.00),
            GEOM_VEC2(1148.00, 484.00),
            GEOM_VEC2(913.00, 717.00),
            GEOM_VEC2(847.00, 718.00),
            GEOM_VEC2(1070.00, 483.00),
        },
        .vertn = 5,
        .hei = 1000.00
    },
    // ninth polygon (quadrilateral)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(690.00, 658.00),
            GEOM_VEC2(807.00, 789.00),
            GEOM_VEC2(564.00, 789.00),
            GEOM_VEC2(690.00, 658.00),
        },
        .vertn = 4,
        .hei = 10000.00
    },
    // tenth polygon (septagonal)
    (struct geom_polygon) {
        .verts = {
            GEOM_VEC2(1306.00, 598.00),
            GEOM_VEC2(1366.00, 624.00),
            GEOM_VEC2(1369.00, 678.00),
            GEOM_VEC2(1306.00, 713.00),
            GEOM_VEC2(1245.00, 673.00),
            GEOM_VEC2(1242.00, 623.00),
            GEOM_VEC2(1306.00, 498.00),
        },
        .vertn = 7,
        .hei = 50000.00
    },
};

// clang-format on
