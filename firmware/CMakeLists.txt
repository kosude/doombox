pico_sdk_init()

# firmware exec
add_executable(doombox
    "main.c"

    "drivers/hal.c"
)
target_link_libraries(doombox
    pico_stdlib
    hardware_pwm
)
