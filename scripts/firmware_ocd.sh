#!/usr/bin/env bash

set -e

usage() {
    printf "Usage: $0 [-h] [-p file]\n"
}
help() {
    printf "Utility tool for interfacing with connected RP2350 via OpenOCD\n"
    usage

    printf "\n"
    printf "  -p file  Program the pico with the specified file instead of"
    printf            " debugging\n"
    printf "  -s       If debugging, run the server only, without GDB\n"
}
error() {
    printf "$0: $1\n"
}

# don't allow prefixing with sudo.
if [ $(id -u) -eq 0 ]; then
    error "This script cannot be called with sudo."
    exit 1
fi

PROJECT_PATH="$(dirname -- "$(dirname -- "$(realpath "$BASH_SOURCE")")")"
GDBINIT=$PROJECT_PATH/scripts/gdbinit

OPENOCD=$(command -v openocd)
GDB=$(command -v gdb)

# default arg values
MODE=2  # 1: program, 2: debug
PROGRAM_PATH= # only set when mode = 1 (programming)
USE_GDB=true # use GDB by default when mode = 0 (debug)

while getopts "p:sh" o; do
    case "$o" in
        p)
            MODE=1
            PROGRAM_PATH=$OPTARG
            ;;
        s)
            USE_GDB=false
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

# -p and -s at the same time is silly
if [ "$MODE" -ne "0" ] && ! $USE_GDB; then
    error "Flag -s was specified despite being in programming mode (-p); this\
 will have no effect."
fi

OPENOCDFLAGS='-f interface/cmsis-dap.cfg -f target/rp2350.cfg'

case "$MODE" in
    # program/flash
    1)
        $OPENOCD $OPENOCDFLAGS -c "adapter speed 5000" \
            -c "program $PROGRAM_PATH verify reset exit"
        ;;
    # server for debugging
    2)
        if $USE_GDB; then
            GDBFLAGS="-x $GDBINIT"

            # run openocd and gdb in parallel
            trap "kill %+" EXIT # kill the openocd process on CTRL+D
            $OPENOCD $OPENOCDFLAGS -c "adapter speed 5000" &
            $GDB $GDBFLAGS
        else
            # just run openocd as server
            $OPENOCD $OPENOCDFLAGS -c "adapter speed 5000"
        fi
        ;;
esac
