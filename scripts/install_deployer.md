# PlatformIO Remote Agent Workday Scheduler

This `install_deployer.sh` script sets up a scheduled service for PlatformIO remote agent usage for `jescore` MCU deployment. With this, one can create a remote host that tests `jescore` on physically connected MCUs for the ultimate runtime test coverage.

## Installation

```bash
git clone https://github.com/jesdev-io/jescore.git
cd jescore/scripts
./install_deployer.sh
```

## What It Does

- Installs systemd services for PlatformIO remote agent
- Schedules agent to run during work hours:
  - **Weekdays (Mon-Fri):** 9:00 AM - 5:00 PM
  - **Weekends (Sat-Sun):** 11:00 AM - 7:00 PM
- Stores credentials securely in `~/.config/platformio-remote/credentials`

## Manual Control

```bash
# Start the agent (if within working hours)
~/.platformio-remote-scripts/manage_agent.sh start

# Stop the agent
~/.platformio-remote-scripts/manage_agent.sh stop

# Check status
~/.platformio-remote-scripts/manage_agent.sh status
```

To activate the remote agent **after** working hours, deactivate the timer and run the start script:
```bash
sudo systemctl stop platformio-remote-agent.timer
sudo systemctl disable platformio-remote-agent.timer
pio remote agent start
```
Dont forget to enable again afterwards!

## Service Management

```bash
# Check service status
sudo systemctl status platformio-remote-agent.service

# View logs
sudo journalctl -u platformio-remote-agent.service -f

# Restart services
sudo systemctl restart platformio-remote-agent.service
sudo systemctl restart platformio-remote-agent.timer
```

## Requirements

- Linux with systemd
- PlatformIO core CLI
- Sudo privileges for installation
- PlatformIO account credentials
