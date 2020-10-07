import os
from io import BytesIO

import serial.tools.list_ports
from serial import Serial, SerialException

BEGIN_OF_MESSAGE = b'\xf0\xbb'
PACKET_SIZE = 70


def init_port_telemetry(port_number, bug_tracker):
    device = None
    for port in serial.tools.list_ports.comports():
        if port.serial_number == port_number:
            device = port.device

    if device is None:
        for count in range(1, 10):
            try:
                port = Serial('/dev/ttyACM' + str(count))
            except SerialException:
                continue
            bug_tracker.emit("Current port: " + str(port))
            return port
        return

    try:
        bug_tracker.emit("Current port: " + str(port))
        return Serial(device)
    except SerialException:
        return


def init_port_hc12(port_number, bug_tracker):
    device = None
    for port in serial.tools.list_ports.comports():
        if port.serial_number == port_number:
            device = port.device

    if device is None:
        for count in range(10):
            try:
                port = Serial('/dev/ttyACM' + str(count))
            except SerialException:
                continue
            bug_tracker.emit("Current port: " + str(port))
            return port
        return

    try:
        return Serial(device)
    except SerialException:
        return


def init_port_aprs():
    pass


def receive_packet_telemetry(port, bug_tracker):
    if port is None:
        return ConnectionError
    port.timeout = 600
    while True:
        try:
            if os.path.exists(port.port):
                start_byte = port.read(1)
                if start_byte == b'':
                        return ConnectionError
                while True:
                    start_byte += port.read(1)
                    bug_tracker.emit(start_byte)
                    if start_byte == BEGIN_OF_MESSAGE:
                        break
                    else:
                        start_byte = start_byte[1:]
            else:
                return ConnectionError
            bug_tracker.emit("Successful received packet!")
            return BytesIO(BEGIN_OF_MESSAGE + port.read(PACKET_SIZE - 2))
        except SerialException:
            return ConnectionError


def receive_packet_hc12(port, bug_tracker):
    if port is None:
        return ConnectionError
    port.timeout = 600
    while True:
        try:
            if os.path.exists(port.port):
                start_byte = port.read(1)
                if start_byte == b'':
                        return ConnectionError
                while True:
                    start_byte += port.read(1)
                    bug_tracker.emit(start_byte)
                    if start_byte == BEGIN_OF_MESSAGE:
                        break
                    else:
                        start_byte = start_byte[1:]
            else:
                return ConnectionError
            bug_tracker.emit("Successful received packet!")
            return BytesIO(BEGIN_OF_MESSAGE + port.read(PACKET_SIZE - 2))
        except SerialException:
            return ConnectionError


def receive_packet_aprs(port):
    pass
