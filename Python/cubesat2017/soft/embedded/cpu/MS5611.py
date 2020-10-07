import struct
import pyb


class MS5611:
    def __init__(self, interface, addr):
        self.i2c = interface
        self.addr = addr
        self.d1 = -999
        self.d2 = -999
        self.p = -999
        if self.i2c.is_ready(self.addr):
            self.i2c.send(0x1E, self.addr)
            pyb.delay(200)
            self.c = [
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xA0))[0],
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xA2))[0],
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xA4))[0],
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xA6))[0],
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xA8))[0],
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xAA))[0],
                struct.unpack('>H', self.i2c.mem_read(2, self.addr, 0xAC))[0],
            ]

    def read_data(self, temp):
        if self.i2c.is_ready(self.addr):
            self.i2c.send(0x40, self.addr)
            pyb.delay(1)
            self.d1 = struct.unpack('>I', b'\x00' + self.i2c.mem_read(3, self.addr, 0x00))[0]
            self.i2c.send(0x50, self.addr)
            pyb.delay(1)
            self.d2 = struct.unpack('>I', b'\x00' + self.i2c.mem_read(3, self.addr, 0x00))[0]
            dt = self.d2 - self.c[5] * 256
            off = self.c[2] * 65536 + (dt * self.c[4]) / 128
            sens = self.c[1] * 32768 + (dt * self.c[3]) / 256
            if temp < 20:
                t2 = (dt * dt) / 2147483648.0
                off2 = 5 * ((temp - 2000) ** 2) / 2
                sens2 = 5 * ((temp - 2000) ** 2) / 4
                if temp < -15:
                    off2 = off2 + 7 * ((temp + 1500.0) ** 2)
                    sens = sens2 + 11 * ((temp + 1500.0) ** 2) / 2
            else:
                t2 = 0
                off2 = 0
                sens2 = 0
            temp = temp - t2
            off = off - off2
            sens = sens - sens2
            self.p = (((self.d1 * sens) / 2097152) - off) / 32768.0
            return abs(self.p * 0.001)
        else:
            return 0.0


