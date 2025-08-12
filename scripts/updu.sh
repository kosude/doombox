#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# updu.sh: DOOMbox Unified Programming and Debugging Utility.
#          Uses the ocdsrv.sh script (with OpenOCD and Telnet) in tandem with
#          a debugging backend to program and debug DOOMbox firmware running
#          on-chip.
#
# Pass the -h flag for argument information.
# -----------------------------------------------------------------------------

ME="updu.sh"

function usage {
    printf "Usage: $ME [-h] [-p|-2] [-R] file\n"
}
function help {
    printf "DOOMbox Unified Programming and Debugging Utility\n"
    printf "Utility tool for interfacing with connected RP2350 via OpenOCD\n"

    printf "\n"
    usage
    printf "\n"

    printf "  -p        Program the pico instead of debugging\n"
    printf "  -2        Two-stage: program and immediately start debugging\n"
    printf "\n"
    printf "  -R        Restart the OpenOCD daemon before communicating, if it"
    printf            " is already up\n"
    printf "\n"
    printf "  -h        Print this help message\n"
}
function error {
    printf "$ME: $1\n"
}

SCRIPT_PATH="$(dirname -- "$(realpath "$BASH_SOURCE")")"
GDBINIT="$SCRIPT_PATH/updu_gdbinit"
OCDSRV="$SCRIPT_PATH/ocdsrv.sh"

# TODO add function to ocdsrv.sh to get ports and get it that way
OCDSRV_GDB_PORT=7510

# default arg values
MODE=1 # from -p,-2 -- 0: program, 1: debug, 2: both
OCDSRV_RESTART=false # from -R

while getopts "p2Rh" o; do
    case "$o" in
        p)
            MODE=0
            ;;
        2)
            MODE=2
            ;;
        R)
            OCDSRV_RESTART=true
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
GDB=gdb
if ! command -v "$GDB" >/dev/null 2>&1; then
    error "$GDB not found in PATH"
    exit 1
fi

# specifying a ELF file path is required
if [ "$#" -ne 1 ]; then
    error "Illegal number of arguments passed"
    usage
    exit 1
fi
# check for the ELF binary at path
if ! [ -f "$PROGRAM_PATH" ]; then
    error "No file at $PROGRAM_PATH"
    exit 1
fi

# kill existing managed openocd server if specified to restart it
if $OCDSRV_RESTART; then
    $OCDSRV kill > /dev/null 2>&1
fi

# function to program the pico
function program {
    $OCDSRV exec -S -c "program $PROGRAM_PATH verify reset"
}

# function to debug the program running on the pico
function debug {
    $GDB -q \
        -x "$GDBINIT" \
        -ex "target extended-remote localhost:$OCDSRV_GDB_PORT" \
        -ex "monitor reset init" \
        "$PROGRAM_PATH"
}

case "$MODE" in
    # program/flash
    0)
        program
        ;;
    # server for debugging
    1)
        debug
        ;;
    # program and debug sequentially
    2)
        program
        debug
        ;;
esac
