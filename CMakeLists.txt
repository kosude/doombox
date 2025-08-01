cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
    message(WARNING "Missing build configuration type! Defaulted to RELEASE, "
                    "reconfigure project with option -DCMAKE_BUILD_TYPE="
                    "(Debug|Release)")
    set(CMAKE_BUILD_TYPE Release)
endif()

# dirs
set(CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE INTERNAL "")

set(CMAKE_MODULE_PATH "${CMAKE_DIR}/mod")

# init project with pico-sdk initialised
find_package(PicoSdk REQUIRED)
project("doombox" C CXX)
pico_sdk_init()

# firmware exec
# TODO move
add_executable(doombox
    "firmware/main.c"
)
target_link_libraries(doombox
    pico_stdlib
)

pico_add_extra_outputs(doombox)
