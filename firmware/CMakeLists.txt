pico_sdk_init()

# firmware exec
add_executable(doombox
    "kernel/drivers/hal.c"
    "kernel/drivers/audio/pz_act.c"
    "kernel/drivers/video/vga.c"

    "kernel/main.c"
    "kernel/panic.c"
)
target_link_libraries(doombox
    pico_stdlib
    hardware_pwm
    hardware_spi
)

# stdio over uart only
pico_enable_stdio_usb(doombox 0)
pico_enable_stdio_uart(doombox 1)

# debug macro
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(doombox PRIVATE _DEBUG)
endif()
