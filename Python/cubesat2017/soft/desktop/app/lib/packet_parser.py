import struct

from PyCRC.CRC16 import CRC16

import lib.exeptions
import lib.gui.bug_tracker

END_OF_MESSAGE = b'\xfa\xdd'
DATA_LENGTH = 61

packet_schema = (
    {'name': 'length', 'size': 1, 'format': 'B'},
    {'name': 'packet_id', 'size': 4, 'format': 'I'},
    {'name': 'sats', 'size': 1, 'format': 'B'},
    {'name': 'time', 'size': 4, 'format': 'I'},
    {'name': 'temp_in', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'temp_out', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'pressure', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'axel_x', 'size': 4, 'format': 'f', 'accuracy': '.5f'},
    {'name': 'axel_y', 'size': 4, 'format': 'f', 'accuracy': '.5f'},
    {'name': 'axel_z', 'size': 4, 'format': 'f', 'accuracy': '.5f'},
    {'name': 'lat', 'size': 4, 'format': 'f', 'accuracy': '.5f'},
    {'name': 'lon', 'size': 4, 'format': 'f', 'accuracy': '.5f'},
    {'name': 'alt', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'hdop', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'vdop', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'bat_volt', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'bat_amp', 'size': 4, 'format': 'f', 'accuracy': '.2f'},
    {'name': 'checksum', 'size': 2, 'format': '>H'},
)


def parse_telemetry(packet, offset, bug_tracker):
    result = {}
    if packet is ConnectionError:
        return ConnectionError

    bug_tracker.emit('Packet of data:' + str(packet.read(70)))

    packet.seek(68)
    if not (packet.read(2) == END_OF_MESSAGE):
        return lib.exeptions.EndOfPacketError

    if offset is not None:
        packet.seek(offset)

    for field in packet_schema:
        values = struct.unpack(
            field['format'],
            packet.read(field['size'])
        )
        value = values[0]

        if 'accuracy' in field:
            value = float(format(value, field['accuracy']))

        result.update({
            field['name']: value
        })

    bug_tracker.emit('Packet time: ' + str(result['time']))
    if not (result['length'] == DATA_LENGTH):
        return lib.exeptions.LengthPacketError

    packet.seek(2)
    crc16_check = CRC16(modbus_flag=True).calculate(packet.read(64))

    if crc16_check != result['checksum']:
        return lib.exeptions.CRC16Error
    bug_tracker.emit('Parsed packet: ' + str(result))
    return result


def parse_aprs(packet):
    pass
