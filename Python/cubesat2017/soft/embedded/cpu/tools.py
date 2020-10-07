import pyb
import struct
import crc16
import os


class SDNotFoundError(Exception):
    pass


def get_temperature(port, i2c):
    raw = []
    port.init(9600, timeout=100)

    for i in range(10):
        raw.append(pyb.ADC('PA4').read())
        pyb.udelay(1)
    volt = (sum(raw) / len(raw)) / 4096 * 3.18
    temp_cpu = (volt - 0.5) * 100
    temp_telem = temp_cpu
    print(temp_cpu)
    if i2c.is_ready(0x2A):
        pyb.delay(4) 
        i2c.send(0xD1, addr=0x2A)
        pyb.delay(4)
        temp_telem = struct.unpack('f', i2c.recv(4, addr=0x2A, timeout=50))[0]
#    pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
#    port.write(b'\xf0\xbb\x01\x02\xd1\x3c\xe1\xfa\xdd')
#    pyb.Pin('PE3', pyb.Pin.OUT_PP).low()
#    data_telem = port.read(14)
#    print('Telemetry temperature packet data:', data_telem)
#    if data_telem is not None and len(data_telem) == 14:
#        temp_telem = struct.unpack('f', data_telem[6:10])[0]
#    print('Telemetry module temperature: ', temp_telem)
    print(temp_telem)
    return (temp_cpu + temp_telem) / 2


def get_temp_out(term):
    
    fault, temp = term.get_temperature_outside()
    print(fault, temp)
    if fault == 0 and temp > -100.0 and temp < 125:
        return temp
    else:
        print('Failed to read thermpcouple data') 
        raw = []
        for i in range(10):
            raw.append(pyb.ADC('PA4').read())
        pyb.udelay(1)
        volt = (sum(raw) / len(raw)) / 4096 * 3.18
        return (volt - 0.5) * 100  



def get_voltamp(packet, port, i2c):   
#    port.init(9600, timeout=50)

#    pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
#    port.write(b'\xf0\xbb\x01\x03\xd0\x6c\x21\xfa\xdd')
#    pyb.Pin('PE3', pyb.Pin.OUT_PP).low()
#    data = port.read()
    packet['amp'] = 0.0
    packet['amp_runcam'] = 0.0
    packet['amp_smth'] = 0.0
    packet['volt'] = 0.0
    if i2c.is_ready(0x3A):
        pyb.delay(4) 
        i2c.send(0xD0, addr=0x3A)
        pyb.delay(4)
        data = i2c.recv(20, addr=0x3A, timeout=50)
        if data is not None:
            packet['amp'] = struct.unpack('f', data[0:4])[0]
            packet['amp_runcam'] = struct.unpack('f', data[4:8])[0]
            packet['amp_smth'] = struct.unpack('f', data[8:12])[0]
            packet['volt'] = struct.unpack('f', data[12:16])[0]
            packet['temp_in'] = (struct.unpack('f', data[16:20])[0] + (packet['temp_in'] * 2.0)) / 3.0


def sent_telemetry(packet, port, rs485):
    port.init(9600, timeout=70)
    rs485.init(9600, timeout=70)
    begin_of_packet = b'\xf0\xbb'
    command = b'\xa0\xf0'
    end_of_packet = b'\xfa\xdd'
    packet_info = b'\x3d' + \
                  struct.pack('I', packet['id']) + \
                  struct.pack('B', packet['sats']) + \
                  struct.pack('I', packet['time']) + \
                  struct.pack('f', packet['temp_in']) + \
                  struct.pack('f', packet['temp_out']) + \
                  struct.pack('f', packet['pres']) + \
                  struct.pack('f', packet['ax']) + \
                  struct.pack('f', packet['ay']) + \
                  struct.pack('f', packet['az']) + \
                  struct.pack('f', packet['lat']) + \
                  struct.pack('f', packet['lon']) + \
                  struct.pack('f', packet['alt']) + \
                  struct.pack('f', packet['hdop']) + \
                  struct.pack('f', packet['vdop']) + \
                  struct.pack('f', packet['volt']) + \
                  struct.pack('f', packet['amp'])
    checksum = struct.pack('>H', crc16.CRC16(modbus_flag=True).calculate(command + packet_info))
    pyb.delay(5)
    port.write(begin_of_packet + command + packet_info + checksum + end_of_packet)
    ans = port.read(9)
    print(ans)
    if ans != b'\xf0\xbb\x02\x01\x42\xa1\x51\xfa\xdd':
        command = b'\x01\x02\xd2'
        pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
        checksum = struct.pack('>H', crc16.CRC16(modbus_flag=True).calculate(command + packet_info))
        rs485.write(begin_of_packet + command + packet_info + checksum + end_of_packet)
        pyb.Pin('PE3', pyb.Pin.OUT_PP).low()
        ans = rs485.read(9)


def check_number_of_files():
    list_dir = os.listdir('/')
    if 'sd2' in list_dir:
        list_files = os.listdir('/sd2')
        counter_list = []
        for name in list_files:
            if name.find('log_cpu') != -1:
                counter_list.append(int(name[name.rfind('log_cpu') + 7:name.find('.')]))
        if len(counter_list) > 0:
            counter_list.sort()
            return counter_list[-1]
        else:
            return 0
    else:
        return 0
   

def write_to_SD(packet, number):
    not_exist_file = False
    begin_of_packet = b'\xbb'
    end_of_packet = b'\xdd'
    packet_info = b'\x3d' + \
                  struct.pack('I', packet['id']) + \
                  struct.pack('B', packet['sats']) + \
                  struct.pack('I', packet['time']) + \
                  struct.pack('f', packet['temp_in']) + \
                  struct.pack('f', packet['temp_out']) + \
                  struct.pack('f', packet['pres']) + \
                  struct.pack('f', packet['ax']) + \
                  struct.pack('f', packet['ay']) + \
                  struct.pack('f', packet['az']) + \
                  struct.pack('f', packet['lat']) + \
                  struct.pack('f', packet['lon']) + \
                  struct.pack('f', packet['alt']) + \
                  struct.pack('f', packet['hdop']) + \
                  struct.pack('f', packet['vdop']) + \
                  struct.pack('f', packet['volt']) + \
                  struct.pack('f', packet['amp'])
    check_list = os.listdir('/sd2')
    if len(check_list) > 0:
        for name in check_list:
            if name == 'log_cpu' + str(number) + '.bin':
                F = open('/sd2/' + name, 'a+')
                statinfo = os.stat('/sd2/' + name)
                if statinfo[6] < 40000:
                    F.write(begin_of_packet + packet_info + end_of_packet)
                    F.close()
                else:
                    F.close()
                    number += 1
                    F = open('/sd2/log_cpu' + str(number) + '.bin', 'w+')
                    F.write(begin_of_packet + packet_info + end_of_packet)
                    F.close()
                return number
    F = open('/sd2/log_cpu' + str(number) + '.bin', 'w+')
    F.write(begin_of_packet + packet_info + end_of_packet)
    F.close()
    return number
