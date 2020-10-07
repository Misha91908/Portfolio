import struct
import pyb 


class DS3231M:
    def __init__(self, interface, addr):
        self.i2c = interface
        self.addr = addr
        self.time = 0

    def adjust(self, year, month, day, hour, minute, second):
        if self.i2c.is_ready(self.addr):
            self.i2c.mem_write(self.dec2bcd(second), self.addr, 0x00)
            self.i2c.mem_write(self.dec2bcd(minute), self.addr, 0x01)
            self.i2c.mem_write(self.dec2bcd(hour), self.addr, 0x02)
            self.i2c.mem_write(0x00, self.addr, 0x03)
            self.i2c.mem_write(self.dec2bcd(day), self.addr, 0x04)
            self.i2c.mem_write(self.dec2bcd(month), self.addr, 0x05)
            self.i2c.mem_write(self.dec2bcd(year - 2000), self.addr, 0x06)
            pyb.delay(10)

    def dec2bcd(self, val):
        return ((val // 10) << 4) + (val % 10)

    def bcd2bin(self, val):
        return ((val >> 4) * 10) + (val & 0x0F)

    def read_data(self):
        if self.i2c.is_ready(self.addr):
            self.i2c.send(0x00, self.addr)
            day_on_month = [0, 31, 59, 90, 120, 151, 181, 22, 243, 273, 304, 334]
            day_on_month_in_leap_year = [0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335]
            second = self.bcd2bin(struct.unpack('B', self.i2c.recv(1, self.addr))[0] & 0x7F)
            minute = self.bcd2bin(struct.unpack('B', self.i2c.recv(1, self.addr))[0])
            hour = self.bcd2bin(struct.unpack('B', self.i2c.recv(1, self.addr))[0])
            self.i2c.recv(1, self.addr)
            day = self.bcd2bin(struct.unpack('B', self.i2c.recv(1, self.addr))[0])
            month = self.bcd2bin(struct.unpack('B', self.i2c.recv(1, self.addr))[0])
            year = self.bcd2bin(struct.unpack('B', self.i2c.recv(1, self.addr))[0]) + 2000
            pyb.delay(5)
            month -= 1
            day -= 1
            if year % 4 != 0:
                time = ((year - 1970) * 31536000) + (day_on_month[month] * 86400) + (((year - 1970) // 4) * 86400) + \
                       (day * 86400) + (hour * 3600) + (minute * 60) + second
            else:
                time = ((year - 1970) * 31622400) + (day_on_month_in_leap_year[month] * 86400) - \
                       (((year - 1970) - ((year - 1970) // 4)) * 86400) + (day * 86400) + \
                       (hour * 3600) + (minute * 60) + second
            return time
        else:
            return 0


