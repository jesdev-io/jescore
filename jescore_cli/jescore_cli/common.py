# -----------------------------------
# Name: common.py
# Author: jake-is-ESD-protected
# Date: 2024-03-01
# Description: Constants for serial comms used on both the client and the MCU with jescore.
# -----------------------------------


"""
General baseline constants
--------------------------

`KNOWN_HOSTS`:                  Types of known MCUs and their HWIDs
`CLI_PREFIX_CLIENT`:            Prefix for messages coming from jescore. Visual only.
`CLI_PREFIX_MCU`                Prefix for the headless CLI (e.g. via putty). Acts as
                                termination of transaction when this CLI is used.
`CLI_CLIENT_IDENTIFIER`:        Prefix for the transmitted message to distinguish
                                between this CLI and the headless CLI.
"""
KNOWN_HOSTS =  {"Generic board with CP2102 USB-to-UART converter": "VID:PID=10C4:EA60",
                "Electrosmith Daisy Seed": "VID:PID=0483:5740",
                "Generic board with CH340 USB-to-UART converter": "VID:PID=1A86:7523",
                "USB enhanced serial CH343": "VID:PID=1A86:55D3",
                "ST LINK": "VID:PID=0483:374B",
                "ST LINK V3": "VID:PID=0483:374E",
                "USB Single Serial": "USB VID:PID=1A86:55D4"}
CLI_PREFIX_CLIENT = "[jescore]:\t"
CLI_PREFIX_MCU = "jescore $ "
CLI_CLIENT_IDENTIFIER = "@py" # unused


"""
Client side flags
-----------------

`CMD_CLIENT_CONNECT_FLAG_PORT`: Flag only used on client side; won't be sent to jescore.
                                Used to let the user choose a COM/tty port:
                                `jescore <command> -p COM<x>`. If not used, the first
                                available port with a known host is used.
`CMD_CLIENT_FLAGS`:             List of known flags.
"""
CMD_CLIENT_CONNECT_FLAG_PORT = "-p"
CMD_CLIENT_DEBUG_FLAG_VERBOSE = "--verbose"
CMD_CLIENT_FLAGS = [CMD_CLIENT_CONNECT_FLAG_PORT,
                    CMD_CLIENT_DEBUG_FLAG_VERBOSE]

"""
Known responses from jescore
----------------------------

`RESPONSE_OK`:                  Standard response from jescore in case of successful transaction with no return string
`RESPONSE_ERR`:                 Standard response from jescore in case of an error during transactions
`RESPONSE_WARN`:                Standard response from jescore in case of a warning during transactions
`RESPONSE_RDY`:                 jescore is ready to receive some form of data or command
`RESPONSE_FNSH`:                jescore finished some sort of data transfer or job
`RESPONSE_TRX_OVER`:            jescore terminates the connection to this CLI
"""
RESPONSE_OK = "OK"
RESPONSE_ERR = "ERR"
RESPONSE_WARN = "WARN"
RESPONSE_RDY = "RDY"
RESPONSE_FNSH = "FNSH"
RESPONSE_TRX_OVER = CLI_PREFIX_MCU

"""
Misc stuff
----------

`CLI_VERBOSE_DEBUG_PREFIX`:     Prefix for print statements if the CLI is set to verbose.
"""
CLI_VERBOSE_DEBUG_PREFIX = "[DEBUG]:\t"