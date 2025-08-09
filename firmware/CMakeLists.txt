pico_sdk_init()

# firmware exec
add_executable(doombox
    "pwm_test.c"
)
target_link_libraries(doombox
    pico_stdlib
    hardware_pwm
)
