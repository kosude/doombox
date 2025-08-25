pico_sdk_init()

# firmware exec
add_executable(doombox
    "kernel/drivers/audio/pz_act.c"
    "kernel/drivers/vga/text/font.c"
    "kernel/drivers/vga/text/puts.c"
    "kernel/drivers/vga/stress.c"
    "kernel/drivers/vga/vga.c"
    "kernel/drivers/vga/vram.c"
    "kernel/drivers/hal.c"

    "kernel/main.c"
    "kernel/panic.c"

    "game/init.c"
    "game/level/lvldata.c"
    "game/renderer/primitive.c"
)
target_link_libraries(doombox
    pico_stdlib
    hardware_pwm
    hardware_dma
    hardware_pio
)
target_include_directories(doombox PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel")

# PIO assembly translation
pico_generate_pio_header(
    doombox
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel/drivers/vga/pio/hsync.pio")
pico_generate_pio_header(
    doombox
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel/drivers/vga/pio/vsync.pio")
pico_generate_pio_header(
    doombox
    "${CMAKE_CURRENT_SOURCE_DIR}/kernel/drivers/vga/pio/video.pio")

# debug macro
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(doombox PRIVATE _DEBUG)
endif()

# stdio over uart only
pico_enable_stdio_usb(doombox 0)
pico_enable_stdio_uart(doombox 1)
