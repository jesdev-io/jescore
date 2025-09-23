from jescore_cli.jescore_cli import CjescoreCli
from jescore_cli.common import CLI_PREFIX_MCU, KNOWN_HOSTS
from serial.tools import list_ports
import platform
import pytest
import re

cli = CjescoreCli()

known_jobs = ["help", "echo", "stats", "logp", "bench"]

device_ports = []
ports = list(list_ports.comports())
for port in ports:
    for host in KNOWN_HOSTS.values():
        if host in port.hwid:
            device_ports.append(cli._CjescoreCli__formatPortForOS(port.name))

def test_cli_init():
    assert cli.os_type == platform.system()

def test_cli_echo():
    for port in device_ports:
        msg = "echo test"
        stat = cli.uartTransceive(msg, port=port)
        assert stat[0] == "test"
        assert stat[1] == CLI_PREFIX_MCU

def test_cli_help():
    for port in device_ports:
        msg = "help"
        stat = cli.uartTransceive(msg, port=port)
        assert stat[-1] == CLI_PREFIX_MCU
        stat = ' '.join(stat) # as single string
        assert "Available jobs" in stat
        assert "(user)" in stat
        assert "(base)" in stat
        assert "echo" in stat
        assert "help" in stat
        assert "stats" in stat

def test_cli_stats():
    for port in device_ports:
        msg = "stats -aa"
        stat = cli.uartTransceive(msg, port=port)
        assert stat[-1] == CLI_PREFIX_MCU
        stat = ' '.join(stat)

        assert "FW" in stat

        assert "name" in stat
        assert "handle" in stat
        assert "memory" in stat
        assert "prio" in stat
        assert "loop" in stat
        assert "instances" in stat
        assert "error" in stat
        
        assert "echo" in stat
        assert "clisrv" in stat
        assert "stats" in stat
        assert "help" in stat
        assert "errorhandler" in stat
        assert "core" in stat 

def test_cli_logp():
    for port in device_ports:
        msg = "logp"
        stat = cli.uartTransceive(msg, port=port)
        assert stat[-1] == CLI_PREFIX_MCU
        stat = ' '.join(stat)

        assert "systime (ms)" in stat
        assert "type" in stat
        assert "name" in stat
        assert "instances" in stat
        assert "error" in stat
        assert "args" in stat
        
        assert "launch" in stat
        assert "stats" in stat

def test_cli_bench():
    for port in device_ports:
        msg = "bench"
        stat = cli.uartTransceive(msg, port=port)
        assert stat[-1] == CLI_PREFIX_MCU
        stat = ' '.join(stat)
        assert "Provide a job name to bench!" in stat

        for job in known_jobs:
            msg = f"bench {job}"
            stat = cli.uartTransceive(msg, port=port)
            stat = ' '.join(stat)

            assert "Roundtrip time" in stat
            assert job in stat
            assert "ms" in stat
            match = re.search(r'\[ (\d+) \]', stat)
            time = int(match[1]) if match else 1
            assert time < 50

if __name__ == "__main__":
    pytest.main()