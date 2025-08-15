cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# enforce build configs
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug) # default to debug mode
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/mod")

# build options
option(DBX_BUILD_FIRMWARE "Build DOOMbox firmware (requires pico-sdk)" ON)

# project init
if (DBX_BUILD_FIRMWARE)
    # pico-sdk cmake config must be included before any project() call
    find_package(PicoSdk REQUIRED)
endif()
project("doombox" C CXX ASM)

# build firmware if specified
if (DBX_BUILD_FIRMWARE)
    add_subdirectory("firmware")
endif()
