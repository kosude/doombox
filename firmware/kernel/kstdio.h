/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

/******************************************************************************
 * kstdio.h
 * Kernel standard input/output. This is only used in a debug configuration,
 * where it is expected that the Pico stdout stream can be recieved by a serial
 * monitor for debugging purposes.
 *****************************************************************************/

#pragma once
#ifndef __kernel__kstdio_h__
#define __kernel__kstdio_h__
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_DEBUG)

#include <pico/stdio.h>
#include <stdio.h>

#define kstdio_init() stdio_init_all()
#define kprintf(...)  printf(__VA_ARGS__)

#else

// stdout macros are no-op in non-debug configurations

#define kstdio_init()
#define kprintf(...)

#endif

#ifdef __cplusplus
}
#endif
#endif
