/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/dma.h>

#include "vga.h"
#include "vram.h"

#include "hsync.pio.h"
#include "video.pio.h"
#include "vsync.pio.h"

// VGA constants
// (must be passed to PIO FIFOs as pioasm cannot set registers above 31)
#define HSYNC_ACTIVE_CYCLES 327
#define VSYNC_ACTIVE_LINES  399
#define VIDEO_HOR_ACTIVE    319

void
vga_init(const struct vga_config cfg)
{
    pio_hw_t *const pio = (cfg.pio) ? pio1 : pio0;

    // load pio programs, offsets are returned
    const uint8_t ofs_hsync = pio_add_program(pio, &hsync_program);
    const uint8_t ofs_vsync = pio_add_program(pio, &vsync_program);
    const uint8_t ofs_video = pio_add_program(pio, &video_program);

    // state machines claimed in the PIO instance
    const uint8_t sm_hsync = 0, sm_vsync = 1, sm_video = 2;
    pio_sm_claim(pio, sm_hsync);
    pio_sm_claim(pio, sm_vsync);
    pio_sm_claim(pio, sm_video);

    // see TIMINGS.txt for timing parameter information
    // + ref for PIO: https://vanhunteradams.com/Pico/VGA/VGA.html
    // ..
    // To get near to the ideal pixel clock frequency, the Pico sys clock
    // freq (150 MHz) can be divided by 5.953125, i.e. CLKDIV INT=5,
    // FRAC=244. This results in a freq of ~25.197 MHz, i.e. one clock
    // cycle ~ 39.688 ns.
    // const uint16_t div_int = 5;
    // const uint8_t div_frac = 244;
    const uint16_t div_int = 11;
    const uint8_t div_frac = 237;

    // init pio synchronisation programs (functions defined in .pio files)
    hsync_program_init(pio,
                       sm_hsync,
                       ofs_hsync,
                       cfg.gpio_hsync,
                       div_int,
                       div_frac);
    vsync_program_init(pio,
                       sm_vsync,
                       ofs_vsync,
                       cfg.gpio_vsync,
                       div_int,
                       div_frac);
    video_program_init(pio, sm_video, ofs_video, cfg.gpio_b2g3r3_base);

    // init dma for VRAM transfers to PIO state machines
    const uint32_t dma_vram = vram_dma_channel_configure(pio, sm_video);

    // put VGA constants - read in PIOs via initial PULL instruction
    pio_sm_put_blocking(pio, sm_hsync, HSYNC_ACTIVE_CYCLES);
    pio_sm_put_blocking(pio, sm_vsync, VSYNC_ACTIVE_LINES);
    pio_sm_put_blocking(pio, sm_video, VIDEO_HOR_ACTIVE);

    // start the pio state machines in time
    pio_enable_sm_mask_in_sync(pio,
                               (1 << sm_hsync) | (1 << sm_vsync)
                                   | (1 << sm_video));
    dma_start_channel_mask(1 << dma_vram);
}
