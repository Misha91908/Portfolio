import micropython

micropython.alloc_emergency_exception_buf(300)

import struct
import math
import pyb
import sdcard
import os
import sys
from ADXL345 import ADXL345
from DS3231 import DS3231M
from GPS import GPS
from MS5611 import MS5611
from RunCam import RunCam
from tools import *
from MAX31856 import MAX31856


data = {'id': 0, 'time': 0, 'sats': 0, 'temp_in': 0.0, 'temp_out': 0.0, 'pres': 0.0, 'ax': 0.0, 'ay': 0.0, 'az': 0.0, 'lat': 0.0, 'lon': 0.0, 'alt': 0.0, 'hdop': 0.0, 'vdop': 0.0, 'volt': 0.0, 'amp': 0.0, 'amp_runcam': 0.0, 'amp_smth': 0.0}
i2c = pyb.I2C(1, pyb.I2C.MASTER)
i2c_bus = pyb.I2C(2, pyb.I2C.MASTER)
uart_telemetry = pyb.UART(2, 9600)
RS485 = pyb.UART(4, 9600)


gps = GPS(pyb.UART(3))
RTC = DS3231M(i2c, addr=0x68)
#RTC.adjust(2018, 6, 26, 18, 0, 0)
bar = MS5611(i2c, addr=0x77)
axel = ADXL345(i2c, addr=0x53)
axel1 = ADXL345(i2c, addr=0x1D)

gps.configure()
zero_g_detection = False
verificator = 0
data_ax = 0.0
data_ay = 0.0
data_az = 0.0

term = MAX31856(pyb.SPI(2, mode=pyb.SPI.MASTER, baudrate=1000000, firstbit=pyb.SPI.MSB, polarity=1, phase=1), pyb.Pin('PB12', pyb.Pin.OUT))
term.temp_skip()
print('End termo init')

def fix_point():
    data['sats'] = 4242


def trigger(inter):
    micropython.schedule(detect, 0)


def detect(n):
    global zero_g_detection
    int_reg = axel.i2c.mem_read(1, axel.addr, 0x30)
    if int_reg == b'\x87':
        zero_g_detection = True


zero_g_extint = pyb.ExtInt(pyb.Pin('PB1', pyb.Pin.IN), pyb.ExtInt.IRQ_RISING, pyb.Pin.PULL_NONE, trigger)


def falling_verification(timer):
    global verificator
    global zero_g_detection
    if zero_g_detection:
        verificator += 1
        zero_g_detection = False


zero_g_timer = pyb.Timer(1)
zero_g_timer.init(freq=10)
zero_g_timer.callback(falling_verification)


camera = RunCam(pyb.Pin('PD14'), pyb.Timer(4, freq=50), 3)
camera_counter = 0
camera_launcher = False
camera_launcher1 = False
camera_make_photo = False
camera_counter1 = 0
camera_counter2 = 0
camera_counter3 = 0
counter = 0

def photo_timer(timer):
    global camera_counter
    global camera_launcher
    global camera_counter1    
    global camera_counter2
    global camera_counter3
    global camera_launcher1
    global camera_make_photo
    camera_counter += 1
    print(camera_counter)
    if camera_make_photo:
        camera_counter3 += 1
        if camera_counter3 == 4:
            camera.make_photo()
        if camera_counter3 == 11:
            camera_make_photo = False
            camera.change_mode_command(RS485, 0, i2c_bus)
            camera_counter3 = 0
    if camera_launcher:
        camera_counter1 += 1
        if camera_counter1 == 20:
            camera.set_standby()
            camera_counter1 = 0
            camera_launcher1 = True
            camera_launcher = False
    if camera_launcher1:
       camera_counter2 += 1
       if camera_counter2 == 5:
           camera.change_mode()
           camera_make_photo = True
           camera_counter2 = 0
           camera_launcher1 = False
    if camera_counter == 60:
        camera_launcher = True
        camera.change_mode_command(RS485, 1, i2c_bus)
        camera_counter = 0


camera_timer = pyb.Timer(2)
camera_timer.init(freq=1)
camera_timer.callback(photo_timer)

sd = sdcard.SDCard(pyb.SPI(1), pyb.Pin('PC4'))
if sd.in_bus:
    print('Mounting sector')
    pyb.mount(sd, '/sd2')
else:
    sd = None
    print('No SD card')
number_of_file = check_number_of_files()

axel.i2c.mem_read(1, axel.addr, 0x30)
print('Main loop start...')


while True:
    data['id'] = counter
    counter += 1
    print('=====================================================================')
    start_iteration = pyb.micros()
    s = pyb.micros()
    ax1, ay1, az1 = axel.read_data()
    ax2, ay2, az2 = axel1.read_data()
    data['ax'] = (ax1 + (-ay2)) / 2
    data['ay'] = (ay1 + ax2) / 2
    data['az'] = (az1 + az2) / 2
#    print('Reading accel data:', pyb.micros() - s)
    s = pyb.micros()
    data['time'] = RTC.read_data()
#    print('Reading RTC data:', pyb.micros() - s)
    s = pyb.micros()
    data['pres'] = bar.read_data(data['temp_in'])
#    print('Reading bar data:', pyb.micros() - s)
    s = pyb.micros()
    data['temp_in'] = get_temperature(RS485, i2c_bus)
    data['temp_out'] = get_temp_out(term)
#    print('Reading temperature data: ', pyb.micros() - s)
    s = pyb.micros()
    gps.parse()
#    print('Parsing GPS data: ', pyb.micros() - s)
    data['lat'] = gps.lat
    data['lon'] = gps.lon
    data['alt'] = gps.alt
    data['hdop'] = gps.hdop
    data['vdop'] = gps.vdop
    data['sats'] = gps.sats
    s = pyb.micros()
    get_voltamp(data, RS485, i2c_bus)
#    print('Reading Powermodule data:', pyb.micros() - s)
    s = pyb.micros()
    sent_telemetry(data, uart_telemetry, RS485)
    if verificator >= 5 and data['alt'] > 3500.0:
        verificator = 0
        data_ax, data_ay, data_az = axel.read_data()
        vector = math.sqrt((data_ax ** 2) + (data_ay ** 2) + (data_az ** 2))
        if vector < 1.095:
            fix_point()
    print('Sending telem data:', pyb.micros() - s)
    print('Packet was sending!')
    print('Packet id:', counter)
    print('Ax Ay Az =', data['ax'], data['ay'], data['az'])
    print('Time =', data['time'])
    print('Temperature inside =', data['temp_in'])
    print('Temperature outside =', data['temp_out'])
    print('Pressure =', data['pres'])
    print('Latitude Longitude Altitude HDOP VDOP sats =', data['lat'], data['lon'], data['alt'], data['hdop'],
          data['vdop'], data['sats'])
    print('Voltage =', data['volt'])
    print('Current =', data['amp'])
    print('Current runcam', data['amp_runcam'])
    print('Current smth', data['amp_smth'])
    if sd is not None:
        number_of_file = write_to_SD(data, number_of_file)
    stop_iteration = pyb.micros()
    print('Loop time:', stop_iteration - start_iteration)
    if stop_iteration - start_iteration < 1000000 and stop_iteration - start_iteration > 0:
        pyb.delay(1000 - int((stop_iteration - start_iteration) / 1000))

