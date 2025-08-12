#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# dump.sh: Dump the contents of the DOOMbox Non-Volatile Memory (NVM) via
#          ocdsrv.sh.
#
# Pass the -h flag for argument information.
# -----------------------------------------------------------------------------

ME="dump.sh"

function usage {
    printf "Usage: $ME [-h] [-n length] [-s offset] [-R]\n"
}
function help {
    printf "Utility for dumping blocks of DOOMbox non-volatile memory (NVM)\n"
    printf "A component of the DOOMbox Project\n"

    printf "\n"
    usage
    printf "\n"

    printf "  -n length  Only dump length bytes (defaults to 1024)\n"
    printf "  -s offset  Skip offset bytes from the beginning\n"
    printf "\n"
    printf "  -R         Restart the OpenOCD daemon before communicating, if"
    printf             " it is already up\n"
    printf "\n"
    printf "  -h         Print this help message\n"
}
function error {
    printf "$ME: $1\n"
}

SCRIPT_PATH="$(dirname -- "$(realpath "$BASH_SOURCE")")"
OCDSRV="$SCRIPT_PATH/ocdsrv.sh"

# default arg values
OCDSRV_RESTART=false # from -R
LENGTH=1024 # from -n
OFFSET=0 # from -s

while getopts "n:s:Rh" o; do
    case "$o" in
        n)
            LENGTH="$OPTARG"
            ;;
        s)
            OFFSET="$OPTARG"
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

# kill existing managed openocd server if specified to restart it
if $OCDSRV_RESTART; then
    $OCDSRV kill > /dev/null 2>&1
fi

# temporary file to store dumped image data
BUFFER=$(mktemp /tmp/doombox-dump.XXXXXXXXXXXX)

# write image bytes to temporary file
$OCDSRV exec -S -c "dump_image $BUFFER $OFFSET $LENGTH" 1>&2

# output bytes
xxd -g1 "$BUFFER"

# clean up
rm "$BUFFER"
