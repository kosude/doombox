#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# ocdsrv.sh: Modal OpenOCD daemon management and command issuer script for
#            DOOMbox hardware communication, based on Telnet.
#
# Pass the -h flag for argument and mode information.
# -----------------------------------------------------------------------------

# TODO handle openocd not running but outdated .ocdsrv_pids file exists

# TODO handle device not plugged in at start
#      and also when unplugged during openocd runtime

ME="ocdsrv.sh"

function usage {
    printf "Usage: $ME start [-h]\n"
    printf "       $ME exec [-h] [-c commands] [-S]\n"
    printf "       $ME kill [-h] [-A]\n"
    printf "       $ME help\n"
}
function help {
    printf "Script to manage a daemonised OpenOCD instance and issue commands"
    printf " on the fly\n"
    printf "A component of the DOOMbox Project\n"

    printf "\n"
    usage
    printf "\n"

    printf "Flags for mode exec:\n"
    printf "  -c cmd  Append cmd as an OpenOCD command to be issued\n"
    printf "  -S      Start a new OpenOCD instance first, if not already up\n"
    printf "\n"

    printf "Flags for mode kill:\n"
    printf "  -A      Kill ALL relevant servers; useful in case .ocdsrv_pids"
    printf          " is lost\n"
    printf "\n"

    printf "General flags:\n"
    printf "  -h      Print this help message (equivalent to mode help)\n"
}
function error {
    printf "$ME: $1\n"
}

SCRIPT_PATH="$(dirname -- "$(realpath "$BASH_SOURCE")")"

PORTS_CFG="$SCRIPT_PATH/ocdsrv_ports.cfg"
PORT_OCD_GDB=7510
PORT_OCD_TCL=7511
PORT_OCD_TELNET=7512

OPENOCD_LOG_PATH="$SCRIPT_PATH/.ocdsrv_log_openocd"

# .ocdsrv_pids:
#       line 1 = PID of started openocd server
PIDFILE_PATH="$SCRIPT_PATH/.ocdsrv_pids"
PIDFILE_LINE_OPENOCD=1

# mode of operation:
#   start: start an openocd server (+ others if configured) if not already up
#             note this can also configure existing servers
#   exec:  issue OpenOCD runtime commands to a running process
#   kill:  kill the openocd server (+ others) created by this script, if up
MODE="$1"
if [ -z "$MODE" ]; then
    error "No mode selected"
    usage
    exit 1
fi
if [ "$MODE" = "help" ]; then
    help
    exit 0
fi
shift 1

# default arg values
EXEC_CMDS=() # from exec: -c
EXEC_AUTOSTART=false # from exec: -S
KILL_ALL_SRVS=false # from kill: -A

# get cli flags
while getopts "c:SAh" o; do
    case "$o" in
        # -c cmd: Append cmd as an OpenOCD command to be issued
        c)
            if [ "$MODE" = "exec" ]; then
                EXEC_CMDS+=("$OPTARG")
            else
                error "-c flag invalid for mode $MODE"
                usage
                exit 1
            fi
            ;;
        # -S: Start a new OpenOCD instance first, if not already up
        S)
            if [ "$MODE" = "exec" ]; then
                EXEC_AUTOSTART=true
            else
                error "-S flag invalid for mode $MODE"
                usage
                exit 1
            fi
            ;;
        A)
            # -A: Kill ALL relevant servers, in case .ocdsrv_pids is lost
            if [ "$MODE" = "kill" ]; then
                KILL_ALL_SRVS=true
            else
                error "-A flag invalid for mode $MODE"
                usage
                exit 1
            fi
            ;;
        h)
            # -h: Print this help message (equivalent to mode help)
            help
            exit 0
            ;;
        *)
            # invalid flags
            usage
            exit 1
            ;;
    esac
done

# assert openocd command
OPENOCD=openocd # TODO any way to assert the RPi fork?
if ! command -v "$OPENOCD" > /dev/null 2>&1; then
    error "OpenOCD not found on PATH (looked for $OPENOCD)"
    exit 1
fi
OCDARGS="-f interface/cmsis-dap.cfg -f target/rp2350.cfg -f \"$PORTS_CFG\""
OCDARGS+=" -c \"adapter speed 5000\""

# assert telnet and expect commands (if needed)
TELNET=telnet
EXPECT=expect
if [ "$MODE" = "exec" ]; then
    if ! command -v "$TELNET" > /dev/null 2>&1; then
        error "Telnet not found on PATH (looked for $TELNET)"
        exit 1
    fi
    if ! command -v "$EXPECT" > /dev/null 2>&1; then
        error "expect not found on PATH (looked for $EXPECT)"
        exit 1
    fi
fi

# look for existing server(s)
OPENOCD_PID=
function load_pids() {
    if [ -f "$PIDFILE_PATH" ]; then
        OPENOCD_PID=$(sed -n "${PIDFILE_LINE_OPENOCD}p" "$PIDFILE_PATH")
    fi
}
load_pids

# Start a new openocd server.
# Returns 0 if successful, 1 if not, 2 if one was already up (so no-op)
function start_ocd {
    if [ -n "$OPENOCD_PID" ]; then
        # pid exists
        return 2
    fi

    eval "$OPENOCD $OCDARGS > \"$OPENOCD_LOG_PATH\" 2>&1 &"
    echo $! > "$PIDFILE_PATH"

    return 0
}

# Issue commands to the OpenOCD server via Telnet and expect.
# Returns 0 if successful, 2 if no OCD server running
function issue_all_telnet {
    # assert that an openocd instance is running
    if [ -z "$OPENOCD_PID" ]; then
        return 2
    fi

    # build expect commands list
    EXPECTCMDS="spawn $TELNET localhost $PORT_OCD_TELNET -4;"
    for CMD in "${EXEC_CMDS[@]}"; do
        EXPECTCMDS+="send \"$CMD\n\";"
    done
    EXPECTCMDS+="send \"exit\n\";" # exit telnet session to avoid interaction
    EXPECTCMDS+="expect eof;" # synchronous command execution

    $EXPECT -c "$EXPECTCMDS"

    return 0
}

# Kill all processes listed in the pidfile
# Return 0 if successful, 2 if pidfile didn't exist
function kill_all {
    if $KILL_ALL_SRVS; then
        # -A specified; kill all instances
        killall $OPENOCD > /dev/null 2>&1 || true
    elif [ ! -f "$PIDFILE_PATH" ]; then
        # -A not specified, and the pidfile is not present
        return 2
    else
        # -A not specified, and pidfile is present, so kill each instance
        while read l; do
            kill "$l" > /dev/null 2>&1 || true
        done < "$PIDFILE_PATH"
    fi

    # always remove pidfile when we're done, if it exists
    rm -f "$PIDFILE_PATH"

    return 0
}

# actual mode execution
case "$MODE" in
    start)
        # start openocd
        start_ocd
        if [ "$?" -eq 2 ]; then
            printf "OpenOCD server already up\n"
        fi
        exit 0
        ;;
    exec)
        # first start a server if specified
        if $EXEC_AUTOSTART; then
            start_ocd
            load_pids # reload process IDs
        fi

        # issue all commands via telnet
        issue_all_telnet
        if [ "$?" -eq 2 ]; then
            error "No managed OpenOCD server found, nowhere to send commands"
            exit 1
        fi
        exit 0
        ;;
    kill)
        kill_all
        if [ "$?" -eq 2 ]; then
            error "PIDfile missing, no processes were killed. Need -A?"
            exit 1
        fi
        ;;
    *)
        error "Invalid mode: $MODE"
        usage
        exit 1
        ;;
esac
