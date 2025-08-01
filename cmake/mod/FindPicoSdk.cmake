# ============================= FINDPICOSDK.CMAKE =============================
# Find and include the Pico SDK cmake config location.
# =============================================================================
# Sets:
#   PICO_SDK_PATH       = location of the Pico SDK source
# =============================================================================

# find PICO_SDK_PATH
if (NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "The PICO_SDK_PATH environment variable is not set")
endif()
set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")

# include cmake funcs
set(PICO_BOARD "pico2")
set(PICO_PLATFORM "rp2350")
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
