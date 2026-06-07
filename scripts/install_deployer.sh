#!/bin/bash

# jescore deployer installer
# This script sets up a scheduled service for jescore MCU deployment 
# in PlatformIO remote with workday (9-5) and weekend (11-7) scheduling

set -e

echo "Checking sudo availability for system-wide service installation..."
if ! command -v sudo &> /dev/null; then
    echo "Error: sudo is not installed. Please install sudo and try again."
    exit 1
fi

if ! sudo -v; then
    echo "Error: Cannot get sudo access. Please ensure you have sudo privileges."
    echo "System-wide service installation requires sudo to write to /etc/systemd/system/"
    exit 1
fi

echo "=========================================="
echo "PlatformIO Remote Agent Installer"
echo "=========================================="
echo ""

if [ "$EUID" -eq 0 ]; then
    echo "Please do NOT run this script as root. It will ask for sudo when needed."
    exit 1
fi

read -p "Enter your PlatformIO username: " PIO_USERNAME
while [ -z "$PIO_USERNAME" ]; do
    read -p "PlatformIO username cannot be empty. Please enter your username: " PIO_USERNAME
done

read -s -p "Enter your PlatformIO password: " PIO_PASSWORD
echo ""
while [ -z "$PIO_PASSWORD" ]; do
    read -s -p "PlatformIO password cannot be empty. Please enter your password: " PIO_PASSWORD
    echo ""
done

read -s -p "Confirm your PlatformIO password: " PIO_PASSWORD_CONFIRM
echo ""

if [ "$PIO_PASSWORD" != "$PIO_PASSWORD_CONFIRM" ]; then
    echo "Error: Passwords do not match. Please try again."
    exit 1
fi

CREDENTIALS_DIR="$HOME/.config/platformio-remote"
echo "Storing credentials in $CREDENTIALS_DIR ..."
mkdir -p "$CREDENTIALS_DIR"

CREDENTIALS_FILE="$CREDENTIALS_DIR/credentials"
cat > "$CREDENTIALS_FILE" <<EOF
PIO_USERNAME="$PIO_USERNAME"
PIO_PASSWORD="$PIO_PASSWORD"
EOF

chmod 600 "$CREDENTIALS_FILE"
echo "Credentials stored securely in $CREDENTIALS_FILE"

SCRIPT_DIR="$HOME/.platformio-remote-scripts"
mkdir -p "$SCRIPT_DIR"

MAIN_SCRIPT="$SCRIPT_DIR/manage_agent.sh"
cat > "$MAIN_SCRIPT" <<'EOF'
#!/bin/bash

# Load credentials
source "$HOME/.config/platformio-remote/credentials"

# Check if we're in working hours
time_in_range() {
    local start=$1
    local end=$2
    local now=$(date +%H%M)
    
    if [ "$start" -le "$end" ]; then
        # Same day range (e.g., 900-1700)
        [ "$now" -ge "$start" ] && [ "$now" -le "$end" ]
    else
        # Overnight range (not used here, but available)
        [ "$now" -ge "$start" ] || [ "$now" -le "$end" ]
    fi
}

# Determine if today is weekend
is_weekend() {
    local day=$(date +%u)  # 1-7, where 6=Saturday, 7=Sunday
    [ "$day" -eq 6 ] || [ "$day" -eq 7 ]
}

# Check if agent should be running
should_run() {
    if is_weekend; then
        # Weekend hours: 11:00-19:00
        time_in_range 1100 1900
    else
        # Weekday hours: 09:00-17:00
        time_in_range 0900 1700
    fi
}

# Login to PlatformIO
pio_login() {
    echo "Logging in to PlatformIO..."
    pio account login -u "$PIO_USERNAME" -p "$PIO_PASSWORD"
}

# Start agent
start_agent() {
    echo "Starting PlatformIO remote agent..."
    pio remote agent start
}

# Stop agent
stop_agent() {
    echo "Stopping PlatformIO remote agent..."
    pio remote agent stop
}

# Check if agent is running
agent_running() {
    pio remote agent status >/dev/null 2>&1
}

# Main management function
manage_agent() {
    if should_run; then
        if ! agent_running; then
            echo "Starting agent (within working hours)"
            pio_login
            start_agent
        fi
    else
        if agent_running; then
            echo "Stopping agent (outside working hours)"
            stop_agent
        fi
    fi
}

# Main logic
case "$1" in
    start)
        manage_agent
        ;;
    stop)
        stop_agent
        ;;
    restart)
        stop_agent
        sleep 5
        manage_agent
        ;;
    status)
        if agent_running; then
            echo "Agent is running"
        else
            echo "Agent is not running"
        fi
        ;;
    *)
        manage_agent
        ;;
esac
EOF

chmod +x "$MAIN_SCRIPT"

echo "Creating systemd service files..."

SERVICE_FILE="/etc/systemd/system/platformio-remote-agent.service"
TIMER_FILE="/etc/systemd/system/platformio-remote-agent.timer"

sudo tee "$SERVICE_FILE" > /dev/null <<EOF
[Unit]
Description=PlatformIO Remote Agent Service
After=network.target

[Service]
Type=simple
User=$USER
ExecStart=$MAIN_SCRIPT
Restart=on-failure
RestartSec=30s
Environment=PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games

[Install]
WantedBy=multi-user.target
EOF

sudo tee "$TIMER_FILE" > /dev/null <<EOF
[Unit]
Description=PlatformIO Remote Agent Scheduling Timer

[Timer]
# Run every hour to check if we should start/stop
OnCalendar=*-*-* *:00:00
Persistent=true

[Install]
WantedBy=timers.target
EOF

echo "Systemd service files created successfully"
echo "Enabling and starting system services..."

if sudo systemctl daemon-reload; then
    echo "Systemd daemon reloaded successfully"
else
    echo "Error: Failed to reload systemd daemon"
    exit 1
fi

if sudo systemctl enable platformio-remote-agent.service; then
    echo "PlatformIO remote agent service enabled"
else
    echo "Error: Failed to enable service"
    exit 1
fi

if sudo systemctl enable platformio-remote-agent.timer; then
    echo "PlatformIO remote agent timer enabled"
else
    echo "Error: Failed to enable timer"
    exit 1
fi

if sudo systemctl start platformio-remote-agent.service; then
    echo "PlatformIO remote agent service started"
else
    echo "Error: Failed to start service"
    exit 1
fi

if sudo systemctl start platformio-remote-agent.timer; then
    echo "PlatformIO remote agent timer started"
else
    echo "Error: Failed to start timer"
    exit 1
fi

echo ""
echo "=========================================="
echo "Installation complete!"
echo "=========================================="
echo ""
echo "The PlatformIO remote agent will now:"
echo "  - Run on weekdays from 9:00 to 17:00"
echo "  - Run on weekends from 11:00 to 19:00"
echo "  - Automatically start/stop based on the schedule"
echo ""
echo "You can manually control the agent with:"
echo "  $MAIN_SCRIPT start   - Start the agent (if within hours)"
echo "  $MAIN_SCRIPT stop    - Stop the agent"
echo "  $MAIN_SCRIPT restart - Restart the agent"
echo "  $MAIN_SCRIPT status  - Check agent status"
echo ""
echo "Systemd services (require sudo to manage):"
echo "  platformio-remote-agent.service - Main agent service"
echo "  platformio-remote-agent.timer   - Hourly scheduling timer"
echo ""
echo "To manage services:"
echo "  sudo systemctl status platformio-remote-agent.service"
echo "  sudo systemctl status platformio-remote-agent.timer"
echo "  sudo journalctl -u platformio-remote-agent.service -f"
echo ""
echo "Credentials are stored securely in: $CREDENTIALS_FILE"
echo ""
echo "The services will run persistently, even when you're not logged in."
