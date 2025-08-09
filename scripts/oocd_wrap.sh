#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# oocd_wrap.sh: Wrapper around OpenOCD that passes common arguments, as well as
#               others passed after the '--' delimiter.
#
# Pass the -h flag for argument information.
# -----------------------------------------------------------------------------

set -e

usage() {
    printf "Usage: $0 [-h] [-K] -- args\n"
}
help() {
    printf "OpenOCD wrapper script for DOOMbox utilities\n"

    printf "\n"
    usage
    printf "\n"

    printf "  -K        Kill all OpenOCD processes first to clear up ports\n"
    printf "  -h        Print this help message\n"
}
error() {
    printf "$0: $1\n"
}

PREKILL=false

while getopts "p2d:Kh" o; do
    case "$o" in
        K)
            PREKILL=true
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

OPENOCD=openocd # TODO any way to assert the RPi fork?
if ! command -v "$OPENOCD" >/dev/null 2>&1; then
    error "OpenOCD not found in PATH\n"
    exit 1
fi

# kill other openocd instances if chosen
if $PREKILL; then
    killall $OPENOCD > /dev/null 2>&1 || true
fi

ARGS="-f interface/cmsis-dap.cfg -f target/rp2350.cfg"
ARGS+=" -c \"adapter speed 5000\""
ARGS+=" ${@@Q}" # see https://stackoverflow.com/a/39463371/12980669

CMD="$OPENOCD $ARGS"

eval "$CMD"
