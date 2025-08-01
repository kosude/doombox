pico_sdk_init()

# firmware exec
add_executable(doombox
    "main.c"
)
target_link_libraries(doombox
    pico_stdlib
)

# uf2, hex, etc
pico_add_extra_outputs(doombox)
