import struct
import pyb


class ADXL345:
    def __init__(self, interface, addr):
        self.i2c = interface
        self.addr = addr
        self.xg = 0
        self.yg = 0
        self.zg = 0
        if self.i2c.is_ready(self.addr):
            self.i2c.mem_write(0x08, self.addr, 0x2D)
            self.i2c.mem_write(0x28, self.addr, 0x21)
            self.i2c.mem_write(0x07, self.addr, 0x28)
            self.i2c.mem_write(0x17, self.addr, 0x29)
            self.i2c.mem_write(0x07, self.addr, 0x2A)
            self.i2c.mem_write(0x04, self.addr, 0x2E)
            self.i2c.mem_write(0xFB, self.addr, 0x2F)
            self.i2c.mem_write(0x0B, self.addr, 0x31)
        self._xoff = -0.23
        self._yoff = 0
        self._zoff = 0.03577027
        self.alpha = 0.5
        self.x = 0
        self.y = 0
        self.z = 0

    def read_data(self):
        if not self.i2c.is_ready(self.addr):
            return 0.0, 0.0, 0.0

        buf = struct.unpack('BBBBBB', self.i2c.mem_read(6, self.addr, 0x32))
        pyb.udelay(200)
        xraw = buf[0] + ((buf[1] & 0x1F) << 8)
        yraw = buf[2] + ((buf[3] & 0x1F) << 8)
        zraw = buf[4] + ((buf[5] & 0x1F) << 8)

        if xraw > 4095:
            xraw = ~(8192 - xraw)
        if yraw > 4095:
            yraw = ~(8192 - yraw)
        if zraw > 4095:
            zraw = ~(8192 - zraw)

        if xraw > 0:
            fxg = xraw * 0.00390625 - self._xoff
        else:
            fxg = xraw * 0.00390625 + self._xoff
        if yraw > 0:
            fyg = yraw * 0.00390625 - self._yoff
        else:
            fyg = yraw * 0.00390625 + self._yoff
        if zraw > 0:
            fzg = zraw * 0.00390625 - self._zoff
        else:
            fzg = zraw * 0.00390625 + self._zoff

        self.x = fxg * self.alpha + (self.xg * (1.0 - self.alpha))
        self.y = fyg * self.alpha + (self.yg * (1.0 - self.alpha))
        self.z = fzg * self.alpha + (self.zg * (1.0 - self.alpha))

        self.xg = self.x
        self.yg = self.y
        self.zg = self.z

        return self.x, self.y, self.z

