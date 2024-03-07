import serial
from serial.tools import list_ports
from time import sleep
from common import *
import numpy as np
import sys


class CjescoreCli:
    def __init__(self, baudrate=115200, verbose=False) -> None:
        self.baudrate = baudrate
        self.verbose = verbose
        self.port = None

    def __vPrint(self, printable):
        if self.verbose:
            print(CLI_VERBOSE_DEBUG_PREFIX, end="")
            print(printable)
    
    def __getPort(self):
        if not self.port:
            self.port = self.portAutoDetect()
        return self.port
    
    def __setPort(self, port):
        self.port = port
    
    def applyArgs(self, args: dict):
        if CMD_CLIENT_CONNECT_FLAG_PORT in args:
            self.__setPort(args[CMD_CLIENT_CONNECT_FLAG_PORT])
            self.__vPrint(f"Set port to <{self.__getPort()}>")
        if CMD_CLIENT_DEBUG_FLAG_VERBOSE in args:
            self.verbose = True

    def portAutoDetect(self):
        ports = list(list_ports.comports())
        for port in ports:
            self.__vPrint(f"Found device: {port.hwid} on port {port.name}")
            for host in KNOWN_HOSTS.values():
                if host in port.hwid:
                    return port.name
        return None
    
    def fromTerminal(self) -> str:
        if len(sys.argv) < 2:  # argv[0] is "jescore", argv[1] is "name"
            print("Usage: jescore <command> [value] [--flag]")
            return None
        args = sys.argv[1:]
        argc = len(args)
        client_args = dict()
        for arg in args.copy():
            if arg in CMD_CLIENT_FLAGS:
                idx = args.index(arg)
                if arg.startswith("--"):
                    client_args[f"{args.pop(idx)}"] = ""
                else:
                    if idx == (argc - 1):
                        print(f"Cannot parse argument of flag <{arg}>. Did you specify it?")
                        return None, None
                    else:
                        client_args[f"{args.pop(idx)}"] = args.pop(idx+1)
        return " ".join(args), client_args

    def __uartTransceive(self, msg: str, waitTime: float = 0.01) -> str:
        self.__vPrint(f"Sending raw string '{msg}' to jes-core")
        ser = serial.Serial(self.__getPort(), baudrate=self.baudrate, timeout=0.01)
        ser.write(msg.encode())
        stat = ""
        returns = []
        print(CLI_PREFIX_CLIENT, end="")
        while RESPONSE_TRX_OVER not in stat:
            stat = ser.readline().decode('utf-8', errors="ignore").strip("\n\r\x00")
            if stat != "":
                if RESPONSE_TRX_OVER in stat:
                    print(RESPONSE_OK)
                else:
                    print(stat)
                returns.append(stat)
        return returns



    def run(self):
        mcu_args, client_args = self.fromTerminal()
        if not mcu_args:
            print(f"Error: Command not specified.")
            return
        if client_args:
            self.applyArgs(client_args)
        stat = cli.__uartTransceive(mcu_args)
        self.__vPrint(stat)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        from unittest import mock
        onTerminal = "jescore blink"
        with mock.patch('sys.argv', onTerminal.split(" ")):
            cli = CjescoreCli(verbose=False)
            cli.run()
    else:
        cli = CjescoreCli(verbose=False)
        cli.run()
