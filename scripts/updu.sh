#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# updu.sh: DOOMbox Unified Programming and Debugging Utility
#
# Pass the -h flag for argument information.
# -----------------------------------------------------------------------------

set -e

usage() {
    printf "Usage: $0 [-h] [-p | -2] [-s] file\n"
}
help() {
    printf "DOOMbox Unified Programming and Debugging Utility\n"
    printf "Utility tool for interfacing with connected RP2350 via OpenOCD\n"
    printf "\n"
    usage

    printf "\n"
    printf "  file  ELF binary to flash/debug\n"
    printf "  -p    Program the pico with the specified file instead of"
    printf        " debugging\n"
    printf "  -2    Two-stage: program and immediately start debugging\n"
}
error() {
    printf "$0: $1\n"
}

# don't allow prefixing with sudo.
if [ $(id -u) -eq 0 ]; then
    error "The UPDU cannot be called with sudo."
    exit 1
fi

PROJECT_PATH="$(dirname -- "$(dirname -- "$(realpath "$BASH_SOURCE")")")"
GDBINIT="$PROJECT_PATH/scripts/updu_gdbinit"

OPENOCD=$(command -v openocd)
GDB=$(command -v gdb)

# default arg values
MODE=1  # 0: program, 1: debug, 2: both

while getopts "p2h" o; do
    case "$o" in
        p)
            MODE=0
            ;;
        2)
            MODE=2
            ;;
        h)
            help
            exit 0
            ;;
        *)
            usage
            exit 1
            ;;
    esac
done
shift $(($OPTIND - 1))

PROGRAM_PATH="$1"
# specifying a path is required
if [ "$#" -ne 1 ]; then
    error "Illegal number of arguments passed"
    usage
    exit 1
fi
# check for the program at path
if ! [ -f "$PROGRAM_PATH" ]; then
    error "No file at $PROGRAM_PATH"
    exit 1
fi

OPENOCDFLAGS='-f interface/cmsis-dap.cfg -f target/rp2350.cfg'

# function to program the pico
program() {
    $OPENOCD $OPENOCDFLAGS \
        -c "adapter speed 5000" \
        -c "program $PROGRAM_PATH verify reset exit"
}

# function to debug the program running on the pico
debug() {
    GDBFLAGS="-q -x $GDBINIT"

    # run openocd and gdb in parallel
    trap "kill %+" EXIT # kill the openocd process on CTRL+D
    $OPENOCD $OPENOCDFLAGS \
        -c "adapter speed 5000" &
    $GDB $GDBFLAGS "$PROGRAM_PATH"
}

case "$MODE" in
    # program/flash
    0)
        printf "$0: Mode: PROGRAM\n"
        program
        ;;
    # server for debugging
    1)
        printf "$0: Mode: DEBUG\n"
        debug
        ;;
    2)
        printf "$0: Mode: 2STAGE\n"
        program
        debug
        ;;
esac
