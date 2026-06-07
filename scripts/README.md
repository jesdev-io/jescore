# PlatformIO Remote Scripts

## 📁 Files

### `pio_remote_scheduler.sh`
**Purpose**: Deterministic workday scheduler for PlatformIO Remote

**Features**:
- Runs PlatformIO Remote only during work hours
- Configurable schedule (edit variables at top)
- Logging to `/var/log/pio_remote_<runner>.log`
- Clean start/stop with lock files

**Usage**:
```bash
# Make executable
chmod +x pio_remote_scheduler.sh

# Edit configuration (top of file)
RUNNER_NAME="runner_1"
WORKDAY_START="08:00"
WORKDAY_END="16:00"
WEEKEND_START="11:00"
WEEKEND_END="19:00"

# Run in background (use nohup or systemd)
nohup ./pio_remote_scheduler.sh > /dev/null 2>&1 &

# Or with systemd (recommended)
# See example systemd service below
```

**Schedule**:
- Weekdays: 8:00-16:00 (8am-4pm)
- Weekends: 11:00-19:00 (11am-7pm)

**Systemd Service Example**:
```ini
[Unit]
Description=PlatformIO Remote Scheduler (%i)
After=network.target

[Service]
Type=simple
User=j
ExecStart=/home/j/repos/jescore/scripts/pio_remote_scheduler.sh
WorkingDirectory=/home/j/repos/jescore
Restart=always
RestartSec=30
Environment=RUNNER_NAME=%i

[Install]
WantedBy=multi-user.target
```

**To use with multiple runners**:
```bash
# Enable for runner_1
sudo systemctl enable pio-remote@runner_1.service
sudo systemctl start pio-remote@runner_1.service

# Enable for runner_2
sudo systemctl enable pio-remote@runner_2.service
sudo systemctl start pio-remote@runner_2.service
```

## 🎯 Deterministic Design

- **No hardcoded paths** - Uses relative paths
- **Configurable** - All settings at top of script
- **Reproducible** - Same behavior everywhere
- **Self-documenting** - Clear log messages
- **Safe** - Lock files prevent conflicts

## 🔄 Workflow

1. **Check current time** → Within work hours?
2. **Start Remote** → If not running
3. **Monitor** → Keep running during work hours
4. **Stop Remote** → When work hours end
5. **Repeat** → Check every 5 minutes

## 🛠️ Customization

To change schedule:
1. Edit variables at top of `pio_remote_scheduler.sh`
2. Restart the service: `sudo systemctl restart pio-remote@runner_1`

To add new runner:
1. Copy script
2. Update `RUNNER_NAME`
3. Create systemd service
4. Enable and start

## 📚 Related

- [PlatformIO Remote Documentation](https://docs.platformio.org/en/latest/plus/remote.html)
- [Systemd Service Documentation](https://www.freedesktop.org/software/systemd/man/systemd.service.html)