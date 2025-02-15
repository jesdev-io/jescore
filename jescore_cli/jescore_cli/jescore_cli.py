import argparse
import serial
from serial.tools import list_ports
from .common import *
import platform


class CjescoreCli:
    def __init__(self, baudrate=115200, verbose=False, port=None) -> None:
        self.baudrate = baudrate
        self.verbose = verbose
        self.port = port
        self.os_type = platform.system()

    def __vPrint(self, printable, end='\n'):
        if self.verbose:
            print(CLI_VERBOSE_DEBUG_PREFIX, printable, end=end)
    
    def __getPort(self):
        if not self.port:
            self.port = self.portAutoDetect()
        return self._formatPortForOS(self.port)

    def __setPort(self, port):
        self.port = port
    
    def applyArgs(self, args):
        if args.port:
            self.__setPort(args.port)
            self.__vPrint(f"Set port to <{self.__getPort()}>")
        if args.verbose:
            self.verbose = True

    def portAutoDetect(self):
        ports = list(list_ports.comports())
        for port in ports:
            self.__vPrint(f"Found device: {port.hwid} on port {port.name}")
            for host in KNOWN_HOSTS.values():
                if host in port.hwid:
                    return port.name
        return None
    
    def _formatPortForOS(self, port):
        """Prefix the port with '/dev/' if the system is Linux."""
        if self.os_type == "Linux" and not port.startswith("/dev/"):
            return f"/dev/{port}"
        return port
    
    def __uartTransceive(self, msg: str, waitTime: float = 0.01) -> str:
        port_name = self.__getPort()
        self.__vPrint(f"Sending raw string '{msg}' to jes-core on port {port_name}")
        ser = serial.Serial(port_name, baudrate=self.baudrate, timeout=waitTime)
        ser.flush()
        ser.setRTS(False)
        ser.write(msg.encode())
        stat = ""
        returns = []
        while RESPONSE_TRX_OVER not in stat:
            stat = ser.readline().decode('utf-8', errors="ignore").strip("\n\r\x00")
            if stat != "":
                if RESPONSE_TRX_OVER in stat:
                    self.__vPrint(RESPONSE_OK)
                returns.append(stat)
        print(CLI_PREFIX_CLIENT, end='')
        for s in returns:
            if CLI_PREFIX_MCU not in s:
                print(s)
        return returns

    def run(self, command):
        if command:
            stat = self.__uartTransceive(command)
            self.__vPrint(f"Received raw string {stat}")
        else:
            print("Error: Command not specified.")


def main():
    parser = argparse.ArgumentParser(description="CLI for jes-core serial communication.")
    parser.add_argument("command", type=str, nargs='?', help="Command to send to jes-core")
    parser.add_argument("-p", "--port", type=str, help="Specify the port for connection")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output")
    parser.add_argument("-b", "--baudrate", type=int, default=115200, help="Baud rate for communication (default: 115200)")

    # Capture both known and unknown arguments
    args, unknown_args = parser.parse_known_args()

    cli = CjescoreCli(baudrate=args.baudrate, verbose=args.verbose, port=args.port)
    cli.applyArgs(args)

    # Join all command parts (including unknown arguments) into a single command string
    command_to_send = ' '.join([args.command] + unknown_args) if args.command else ' '.join(unknown_args)
    
    cli.run(command_to_send)


if __name__ == "__main__":
    main()
