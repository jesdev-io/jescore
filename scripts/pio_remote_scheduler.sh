#!/bin/bash

RUNNER_NAME="runner_1"  # Match with .github/runners.yml
WORKDAY_START="08:00"
WORKDAY_END="16:00"
WEEKEND_START="11:00"
WEEKEND_END="19:00"

LOG_FILE="${HOME}/pio_remote_${RUNNER_NAME}.log"
LOCK_FILE="/tmp/pio_remote_${RUNNER_NAME}.lock"

log() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] [$RUNNER_NAME] $1" | tee -a "$LOG_FILE"
}

is_work_hours() {
    DAY=$(date +%u)  # 1-7 (Mon-Sun)
    HOUR=$(date +%H)
    MINUTE=$(date +%M)
    CURRENT_TIME="$(printf "%02d:%02d" $HOUR $MINUTE)"
    if [ $DAY -le 5 ]; then
        # Weekday
        [ "$CURRENT_TIME" \> "$WORKDAY_START" ] && [ "$CURRENT_TIME" \< "$WORKDAY_END" ]
    else
        # Weekend
        [ "$CURRENT_TIME" \> "$WEEKEND_START" ] && [ "$CURRENT_TIME" \< "$WEEKEND_END" ]
    fi
}

start_remote() {
    if [ -f "$LOCK_FILE" ]; then
        log "Remote already running"
        return
    fi
    log "Starting PlatformIO Remote for $RUNNER_NAME"
    touch "$LOCK_FILE"
    ~/.local/bin/pio remote agent start --runner "$RUNNER_NAME"
    if [ $? -eq 0 ]; then
        log "PlatformIO Remote started successfully"
    else
        log "Failed to start PlatformIO Remote"
        rm -f "$LOCK_FILE"
        return 1
    fi
}

stop_remote() {
    if [ ! -f "$LOCK_FILE" ]; then
        log "Remote not running"
        return
    fi
    log "Stopping PlatformIO Remote for $RUNNER_NAME"
    ~/.local/bin/pio remote agent stop --runner "$RUNNER_NAME"
    rm -f "$LOCK_FILE"
    log "✅ PlatformIO Remote stopped"
}

main() {
    log "Starting PlatformIO Remote Scheduler for $RUNNER_NAME"
    log "Work hours: Weekdays $WORKDAY_START-$WORKDAY_END, Weekends $WEEKEND_START-$WEEKEND_END"
    while true; do
        if is_work_hours; then
            if [ ! -f "$LOCK_FILE" ]; then
                start_remote
            fi
        else
            if [ -f "$LOCK_FILE" ]; then
                stop_remote
            fi
        fi
        sleep 300
    done
}

main