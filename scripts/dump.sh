#!/usr/bin/env bash

# TODO figure out how to daemonise openocd and keep it local to this script to
#      clean up AND how to then communicate to it via telnet

set -e

usage() {
    printf "Usage: $0 [-h] [-K]\n"
}
help() {
    usage
    printf "\n"

    printf "  -n length   Only dump length bytes\n"
    printf "  -s offset   Skip offset bytes from the beginning\n"
    printf "  -K          Kill ALL existing OpenOCD processes first\n"
    printf "\n"
    printf "  -h        Print this help message\n"
}
error() {
    printf "$0: $1\n"
}

OCDKILL_FLAG="" # -K optionally passed to oocd_wrap.sh
LENGTH= # from -n
OFFSET= # from -s

while getopts "n:s:Kh" o; do
    case "$o" in
        n)
            LENGTH="$OPTARG"
            ;;
        s)
            OFFSET="$OPTARG"
            ;;
        K)
            OCDKILL_FLAG="-K"
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

PROJECT_PATH="$(dirname -- "$(dirname -- "$(realpath "$BASH_SOURCE")")")"
OPENOCD_WRAP="$PROJECT_PATH/scripts/oocd_wrap.sh"

# check for telnet
TELNET=telnet
if ! command -v "$TELNET" >/dev/null 2>&1; then
    error "$TELNET not found on PATH"
    exit 1
fi

# start openocd instance
$OPENOCD_WRAP $OCDKILL_FLAG & 1>&2

# connect via telnet
# TODO
$TELNET localhost 4444 << EOF
image_dump
EOF
