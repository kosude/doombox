/*
 * Copyright (c) 2025 Jack Bennett.
 * All Rights Reserved.
 *
 * See the LICENCE file for more information.
 */

// TODO check includes needed
#include <hardware/dma.h>
#include <pico/stdlib.h>
#include <string.h>

#include "vga.h"

#include "hsync.pio.h"
#include "video.pio.h"
#include "vsync.pio.h"

#include "kstdio.h"

// TODO allow choosing spi/pio instances with outputs

// SPI 0 reserved for video data transmission
#define SPI spi0
// PIO 0 reserved for VGA synchronisation
#define PIO pio0

// VGA timing constants
// Must be passed to PIO FIFOs as pioasm cannot set registers above 31.
#define HSYNC_ACTIVE_CYCLES 655 // (640 + 8 + 8) - (1 to account for MOV)
#define VSYNC_ACTIVE_LINES  479 // (480) - (1 to account for first wait)

// Video signal driver constants
#define VIDEO_HOR_ACTIVE 639 // 640 - 1

#define TXCOUNT 307200
uint8_t arr[TXCOUNT];
uint8_t *arrptr = arr;

static size_t ii = 0;

// TODO remove
static void
__testirq()
{
    gpio_put(25, 1);

    if (ii >= 307000) {
        ii = 0;
    }

    printf("%d %d %d %d\n",
           (arr[ii] & 0x01) != 0,
           (arr[ii] & 0x02) != 0,
           (arr[ii] & 0x04) != 0,
           (arr[ii] & 0x08) != 0);

    for (size_t i = 0; i < 640; i++, ii++) {
        gpio_put(2 + 7, (arr[ii] & 0x01) != 0);
        gpio_put(2 + 6, (arr[ii] & 0x02) != 0);
        gpio_put(2 + 5, (arr[ii] & 0x04) != 0);
        gpio_put(2 + 4, (arr[ii] & 0x08) != 0);
        gpio_put(2 + 3, (arr[ii] & 0x10) != 0);
        gpio_put(2 + 2, (arr[ii] & 0x20) != 0);
        gpio_put(2 + 1, (arr[ii] & 0x40) != 0);
        gpio_put(2 + 0, (arr[ii] & 0x80) != 0);
    }

    gpio_put(25, 0);

    // // start
    // gpio_put(4, 0);

    // // for (size_t i = 0; i < 640 / 3; i++) {
    // //     // A0 and A1 both tied to gnd, so MCP client address is 0100000x
    // //     // where x is 0 for write and 1 for read. (so for writing, 0x40)
    // //     // The GPIO register is on address 0x09.
    // //     spi_write_blocking(SPI, (uint8_t[]){ 0x40 }, 1);
    // //     spi_write_blocking(SPI, (uint8_t[]){ 0x09 }, 1);
    // //     spi_write_blocking(SPI, (uint8_t[]){ 0b00111000 }, 1);
    // // }

    // // stop
    // gpio_put(4, 1);

    // pixi += 640;
    // if (pixi >= 307200) {
    //     pixi = 0;
    // }

    // pio_interrupt_clear(PIO, 2);
}

void
vga_init(const struct vga_outputs op)
{
    // TODO temp+
    // for (size_t i = 0; i < 8; i++) {
    //     gpio_init(op.gpio_rgb8 + i);
    //     gpio_set_dir(op.gpio_rgb8 + i, GPIO_OUT);
    //     gpio_put(op.gpio_rgb8 + i, 1);
    // }
    // return;

    // see TIMINGS.txt for timing parameter information
    // + ref for PIO: https://vanhunteradams.com/Pico/VGA/VGA.html

    // To get near to the ideal pixel clock frequency, the Pico sys clock freq
    // (150 MHz - #TODO will this change?) can be divided by 5.953125, i.e.
    // CLKDIV INT=5, FRAC=244.
    // This results in a freq of ~25.197 MHz, i.e. one clock cycle ~ 39.688 ns.
    const uint16_t pxdiv_int = 5;
    const uint8_t pxdiv_frac = 244;
    // const uint16_t pxdiv_int = 5;
    // const uint8_t pxdiv_frac = 0;

    // load pio programs, offsets are returned
    const uint8_t ofs_hsync = pio_add_program(PIO, &hsync_program);
    const uint8_t ofs_vsync = pio_add_program(PIO, &vsync_program);
    const uint8_t ofs_video = pio_add_program(PIO, &video_program);

    // state machines claimed in the PIO instance
    const uint8_t sm_hsync = 0, sm_vsync = 1, sm_video = 2;
    pio_sm_claim(PIO, sm_hsync);
    pio_sm_claim(PIO, sm_vsync);
    pio_sm_claim(PIO, sm_video);

    // init pio synchronisation programs (functions defined in .pio files)
    hsync_program_init(PIO,
                       sm_hsync,
                       ofs_hsync,
                       op.gpio_hsync,
                       pxdiv_int,
                       pxdiv_frac);
    vsync_program_init(PIO,
                       sm_vsync,
                       ofs_vsync,
                       op.gpio_vsync,
                       pxdiv_int,
                       pxdiv_frac);
    video_program_init(PIO, sm_video, ofs_video, op.gpio_rgb8);
    // spictl_program_init(PIO, sm_spictl, ofs_spictl, op.gpio_scl0,
    // op.gpio_sdo0); spirgb_program_init(PIO, sm_spirgb, ofs_spirgb,
    // op.gpio_scl0, op.gpio_sdo0);

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
                          &PIO->txf[sm_video], // write address (RGB PIO TX
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
    pio_sm_put_blocking(PIO, sm_hsync, HSYNC_ACTIVE_CYCLES);
    pio_sm_put_blocking(PIO, sm_vsync, VSYNC_ACTIVE_LINES);
    pio_sm_put_blocking(PIO, sm_video, VIDEO_HOR_ACTIVE);

    // TODO temp only
    // pio_set_irq0_source_enabled(PIO, pis_interrupt1, true);
    // irq_set_exclusive_handler(PIO0_IRQ_0, __testirq);
    // irq_set_enabled(PIO0_IRQ_0, true);

    memset(arr, 0x00, 307200);

    size_t posx = 0, posy = 0;

    // loop through columns (inc blue)
    // for (uint8_t b = 0; b < 4; b++) {
    //     // encode rgb value (BBGGGRRR)
    //     // TODO it may be RRRGGGBB
    //     uint8_t rgb = (b << 6); // | (g << 3) | r;
    //     // uint8_t rgb = (r << 6) | (g << 3) | b;

    //     // draw a 40x30 block of colour rgb
    //     for (size_t i = 0; i < 30; i++) {
    //         for (size_t j = 0; j < 40; j++) {
    //             arr[((posy + i) * 640) + (posx + j)] = rgb;
    //         }
    //     }

    //     posx += 40;
    // }

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

    // for (size_t y = 0; y < 480; y++) {
    //     for (size_t x = 0; x < 640; x++) {
    //         if (y < 120) {
    //             if (x < 160)
    //                 arr[y * 640 + x] = 0b00000111;
    //             else if (x < 320)
    //                 arr[y * 640 + x] = 0b00001111;
    //             else if (x < 480)
    //                 arr[y * 640 + x] = 0b00011111;
    //             else
    //                 arr[y * 640 + x] = 0b00111111;
    //         }
    //     }
    // }

    // start the pio state machines in time
    pio_enable_sm_mask_in_sync(PIO,
                               (1 << sm_hsync) | (1 << sm_vsync)
                                   | (1 << sm_video));

    dma_start_channel_mask(1 << rgb_chan_0);
}
