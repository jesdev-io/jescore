#!/bin/bash

# Simple PlatformIO Remote Scheduler
# Just starts/stops agent based on work hours

RUNNER_NAME="runner_1"
LOG_FILE="${HOME}/pio_remote_${RUNNER_NAME}.log"

log() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [$RUNNER_NAME] $1" | tee -a "$LOG_FILE"
}

is_work_hours() {
    DAY=$(date +%u)
    HOUR=$(date +%H)
    
    if [ $DAY -le 5 ]; then
        [ $HOUR -ge 8 ] && [ $HOUR -lt 16 ]
    else
        [ $HOUR -ge 11 ] && [ $HOUR -lt 19 ]
    fi
}

start_agent() {
    log "Starting PlatformIO Remote agent"
    ~/.local/bin/pio remote agent start --runner "$RUNNER_NAME" > /dev/null 2>&1 &
    sleep 3
    if pgrep -f "pio remote agent" > /dev/null; then
        log "✅ Agent started"
        return 0
    else
        log "❌ Failed to start agent"
        return 1
    fi
}

stop_agent() {
    log "Stopping PlatformIO Remote agent"
    pkill -f "pio remote agent" > /dev/null 2>&1
    ~/.local/bin/pio remote agent stop --runner "$RUNNER_NAME" > /dev/null 2>&1
    log "✅ Agent stopped"
}

# Main loop
log "Starting simple scheduler for $RUNNER_NAME"

while true; do
    if is_work_hours; then
        if ! pgrep -f "pio remote agent" > /dev/null; then
            start_agent
        fi
    else
        if pgrep -f "pio remote agent" > /dev/null; then
            stop_agent
        fi
    fi
    
    sleep 300  # Check every 5 minutes
 done