import argparse
import serial
from serial.tools import list_ports
from .common import *
import platform


class CjescoreCli:
    def __init__(self, baudrate=115200, verbose=False, port=None) -> None:
        self.os_type = platform.system()
        self.baudrate = baudrate
        self.verbose = verbose
        _port = port if port else self.portAutoDetect()
        if not _port:
            print("No jescore-enabled device detected!")
            exit()
        self.port = self.__formatPortForOS(port if port else self.portAutoDetect())
        
    def __vPrint(self, printable, end='\n'):
        if self.verbose:
            print(CLI_VERBOSE_DEBUG_PREFIX, printable, end=end)

    def discoverPorts(self):
        ports = list(list_ports.comports())
        for port in ports:
            for device, host in KNOWN_HOSTS.items():
                if host in port.hwid:
                    print(f"{device}({host}) on port {port.name}")

    def portAutoDetect(self):
        ports = list(list_ports.comports())
        for port in ports:
            self.__vPrint(f"Found device: {port.hwid} on port {port.name}")
            for host in KNOWN_HOSTS.values():
                if host in port.hwid:
                    return port.name
        return None
    
    def __formatPortForOS(self, port):
        """Prefix the port with '/dev/' if the system is Linux."""
        if self.os_type == "Linux" and not port.startswith("/dev/"):
            return f"/dev/{port}"
        return port
    
    def uartTransceive(self, msg: str, port: str = None, waitTime: float = 0.01) -> str:
        try:
            port_name = port if port else self.port
            self.__vPrint(f"Sending raw string '{msg}' to jescore on port {port_name}")
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
            if len(returns) != 1:
                print(CLI_PREFIX_CLIENT)
            for s in returns:
                if CLI_PREFIX_MCU not in s:
                    print(s)
            return returns
        except KeyboardInterrupt:
            self.__vPrint(f"Closing port {port_name}.")
            return
    
    def uartReceive(self, port: str = None, waitTime: float = 0.01):
        try:
            port_name = port if port else self.port
            self.__vPrint(f"Listening on port {port_name}...")
            ser = serial.Serial(port_name, baudrate=self.baudrate, timeout=waitTime)
            ser.flush()
            ser.setRTS(False)
            while(1):
                stat = ser.readline().decode('utf-8', errors="ignore").strip("\n\r\x00")
                if stat != "":
                    print(stat)
        except KeyboardInterrupt:
            self.__vPrint(f"Closing port {port_name}.")
            return

    def run(self, command):
        if command:
            stat = self.uartTransceive(command)
            self.__vPrint(f"Received raw string {stat}")
        else:
            print("Error: Command not specified.")


def main():
    parser = argparse.ArgumentParser(description="CLI for jescore serial communication.")
    parser.add_argument("command", type=str, nargs='?', help="Command to send to jescore")
    parser.add_argument("-p", "--port", type=str, help="Specify the port for connection")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose output")
    parser.add_argument("-b", "--baudrate", type=int, default=115200, help="Baud rate for communication (default: 115200)")
    parser.add_argument("-d", "--discover", action="store_true", help="Discover connected devices known to jescore")
    parser.add_argument("-l", "--listen", action="store_true", help="Listen to the given UART stream")

    # Capture both known and unknown arguments
    args, unknown_args = parser.parse_known_args()

    cli = CjescoreCli(baudrate=args.baudrate, verbose=args.verbose, port=args.port)

    if args.discover:
        cli.discoverPorts()
        return
    
    if args.listen:
        cli.uartReceive()
        return

    # Join all command parts (including unknown arguments) into a single command string
    command_to_send = ' '.join([args.command] + unknown_args) if args.command else ' '.join(unknown_args)
    
    cli.run(command_to_send)


if __name__ == "__main__":
    main()
