#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# updu.sh: DOOMbox Unified Programming and Debugging Utility
#
# Pass the -h flag for argument information.
# -----------------------------------------------------------------------------

set -e

usage() {
    printf "Usage: $0 [-h] [-p|-2] [-d <gdb|none>] file\n"
}
help() {
    printf "DOOMbox Unified Programming and Debugging Utility\n"
    printf "Utility tool for interfacing with connected RP2350 via OpenOCD\n"
    printf "\n"
    usage

    printf "\n"
    printf "  -p        Program the pico with the specified file instead of"
    printf            " debugging\n"
    printf "  -2        Two-stage: program and immediately start debugging\n"
    printf "  -d name   Specify debugger to use (default: gdb)\n"
    printf "  -K        Kill ALL existing OpenOCD processes before starting"
    printf "\n"
    printf "  -h        Print this help message\n"
}
error() {
    printf "$0: $1\n"
}
assert_cmd() {
    if ! command -v "$1" >/dev/null 2>&1; then
        error "$1 not found in PATH"
        exit 1
    fi
}

# don't allow prefixing with sudo.
if [ $(id -u) -eq 0 ]; then
    error "The UPDU cannot be called with sudo."
    exit 1
fi

PROJECT_PATH="$(dirname -- "$(dirname -- "$(realpath "$BASH_SOURCE")")")"
GDBINIT="$PROJECT_PATH/scripts/updu_gdbinit"

# default arg values
MODE=1  # 0: program, 1: debug, 2: both
DBG_NAME="gdb"
KILLOCD=false

while getopts "p2d:Kh" o; do
    case "$o" in
        p)
            MODE=0
            ;;
        2)
            MODE=2
            ;;
        d)
            DBG_NAME="$OPTARG"
            ;;
        K)
            KILLOCD=true
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
# positional arguments
PROGRAM_PATH="$1"

# dependency checks
OPENOCD=openocd # TODO any way to assert the RPi fork?
GDB=gdb
assert_cmd $OPENOCD
if [ "$DBG_NAME" = "gdb" ]; then
    assert_cmd $GDB
fi

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

# kill other openocd instances if chosen
if $KILLOCD; then
    killall -q $OPENOCD || true
fi

OPENOCDFLAGS="-f interface/cmsis-dap.cfg -f target/rp2350.cfg \
              -c 'adapter speed 5000'"

# function to program the pico
program() {
    eval "$OPENOCD $OPENOCDFLAGS \
          -c \"program $PROGRAM_PATH verify reset exit\""
}

# function to debug the program running on the pico
debug() {
    # if just running the openocd server then forget about debugging after
    if [ "$DBG_NAME" = "none" ]; then
        eval "$OPENOCD $OPENOCDFLAGS"
        return
    fi

    DBGCMD=
    case "$DBG_NAME" in
        gdb)
            DBGCMD="$GDB -q -x $GDBINIT \"$PROGRAM_PATH\""
            ;;
        none)
            ;;
        *)
            error "Invalid debugger name $DBG_NAME"
            usage
            exit 1
            ;;
    esac

    # run openocd and debugger in parallel
    trap "kill %+" EXIT # kill the openocd process on CTRL+D
    eval "$OPENOCD $OPENOCDFLAGS" &
    eval "$DBGCMD"
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
    # program and debug sequentially
    2)
        printf "$0: Mode: 2STAGE\n"
        program
        debug
        ;;
esac
