/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

#include <hardware/dma.h>
#include <string.h> // TODO only needed for arr memset

#include "vga.h"

#include "hsync.pio.h"
#include "video.pio.h"
#include "vsync.pio.h"

// VGA constants
// (must be passed to PIO FIFOs as pioasm cannot set registers above 31)
#define HSYNC_ACTIVE_CYCLES 655 // (640 + 8 + 8) - (1 to account for MOV)
#define VSYNC_ACTIVE_LINES  479 // (480) - (1 to account for first wait)
#define VIDEO_HOR_ACTIVE    639 // 640 - 1 (num pixels per line)

#define TXCOUNT 307200
uint8_t arr[TXCOUNT];
uint8_t *arrptr = arr;

void
vga_init(const struct vga_config cfg)
{
    // see TIMINGS.txt for timing parameter information
    // + ref for PIO: https://vanhunteradams.com/Pico/VGA/VGA.html

    // To get near to the ideal pixel clock frequency, the Pico sys clock freq
    // (150 MHz) can be divided by 5.953125, i.e. CLKDIV INT=5, FRAC=244.
    // This results in a freq of ~25.197 MHz, i.e. one clock cycle ~ 39.688 ns.
    const uint16_t div_int = 5;
    const uint8_t div_frac = 244;

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

    // DMA channels - 0 sends color data, 1 reconfigures and restarts 0
    int rgb_chan_0 = 0;
    int rgb_chan_1 = 1;

    // Channel Zero (sends color data to PIO VGA machine)
    dma_channel_config c0 =
        dma_channel_get_default_config(rgb_chan_0);         // default configs
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8); // 8-bit txfers
    channel_config_set_read_increment(&c0, true);   // yes read incrementing
    channel_config_set_write_increment(&c0, false); // no write incrementing
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2); // DREQ_PIO0_TX2 pacing (FIFO)
    channel_config_set_chain_to(&c0, rgb_chan_1); // chain to other channel

    dma_channel_configure(rgb_chan_0, // Channel to be configured
                          &c0,        // The configuration we just created
                          &pio->txf[sm_video], // write address (RGB PIO TX
                                               // FIFO)
                          &arr,    // The initial read address (pixel
                                   // color array)
                          TXCOUNT, // Number of transfers; in this case each is
                                   // 1 byte.
                          false    // Don't start immediately.
    );

    // Channel One (reconfigures the first channel)
    dma_channel_config c1 =
        dma_channel_get_default_config(rgb_chan_1);          // default configs
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32); // 32-bit txfers
    channel_config_set_read_increment(&c1, false);  // no read incrementing
    channel_config_set_write_increment(&c1, false); // no write incrementing
    channel_config_set_chain_to(&c1, rgb_chan_0);   // chain to other channel

    dma_channel_configure(rgb_chan_1, // Channel to be configured
                          &c1,        // The configuration we just created
                          &dma_hw->ch[rgb_chan_0].read_addr, // Write address
                                                             // (channel 0 read
                                                             // address)
                          &arrptr, // Read address (POINTER TO AN
                                   // ADDRESS)
                          1,    // Number of transfers, in this case each is 4
                                // byte
                          false // Don't start immediately.
    );

    // put VGA constants - read in PIOs via initial PULL instruction
    pio_sm_put_blocking(pio, sm_hsync, HSYNC_ACTIVE_CYCLES);
    pio_sm_put_blocking(pio, sm_vsync, VSYNC_ACTIVE_LINES);
    pio_sm_put_blocking(pio, sm_video, VIDEO_HOR_ACTIVE);

    memset(arr, 0x00, 307200);

    size_t posx = 0, posy = 0;

    // loop through blocks (incr red)
    for (uint8_t r = 0; r < 8; r++) {
        // loop through rows (incr green)
        for (uint8_t g = 0; g < 8; g++) {
            // loop through columns (inc blue)
            for (uint8_t b = 0; b < 4; b++) {
                // encode rgb value (BBGGGRRR)
                uint8_t rgb = (b << 6) | (g << 3) | r;

                // draw a 40x30 block of colour rgb
                for (size_t i = 0; i < 30; i++) {
                    for (size_t j = 0; j < 40; j++) {
                        arr[((posy + i) * 640) + (posx + j)] = rgb;
                    }
                }

                posx += 40;
            }
            posx -= 160;
            posy += 30;
        }

        if (r != 3) {
            posx += 160;
            posy -= 240;
        } else {
            posx = 0;
        }
    }

    // start the pio state machines in time
    pio_enable_sm_mask_in_sync(pio,
                               (1 << sm_hsync) | (1 << sm_vsync)
                                   | (1 << sm_video));

    dma_start_channel_mask(1 << rgb_chan_0);
}
