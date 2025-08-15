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
