from jescore_cli.jescore_cli import CjescoreCli
from jescore_cli.common import CLI_PREFIX_MCU
import platform
import pytest

cli = CjescoreCli()

def test_cli_init():
    assert cli.os_type == platform.system()

def test_cli_echo():
    cli.uartTransceive("provoke flush")
    msg = "echo test"
    stat = cli.uartTransceive(msg)
    assert stat[0] == "test"
    assert stat[1] == CLI_PREFIX_MCU

def test_cli_help():
    msg = "help"
    stat = cli.uartTransceive(msg)
    assert stat[-1] == CLI_PREFIX_MCU
    stat = ' '.join(stat) # as single string
    assert "Available jobs" in stat
    assert "(user)" in stat
    assert "(base)" in stat
    assert "echo" in stat
    assert "help" in stat
    assert "stats" in stat

def test_cli_stats():
    msg = "stats"
    stat = cli.uartTransceive(msg)
    assert stat[-1] == CLI_PREFIX_MCU
    stat = ' '.join(stat)
    assert "name" in stat
    assert "handle" in stat
    assert "memory" in stat
    assert "prio" in stat
    assert "loop" in stat
    assert "instances" in stat
    assert "error" in stat
    
    assert "echo" in stat
    assert "cliserver" in stat
    assert "stats" in stat
    assert "help" in stat
    assert "headerprint" in stat
    assert "initcli" in stat
    assert "errorhandler" in stat
    assert "core" in stat 

if __name__ == "__main__":
    pytest.main()