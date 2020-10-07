import pyb


class MAX31856:
    def __init__(self, spi, cs):
        self.spi = spi
        self.cs = cs
        self.cs.high()

        self.REGISTER_CR0 = 0
        # print('REGISTER_CR0 = ', CR0_INIT())
        # print(readRegister8(REGISTER_CR0))
        self.write_register(self.REGISTER_CR0, self.CR0_INIT())
        # print(readRegister8(REGISTER_CR0))

        self.REGISTER_CR1 = 1
        # print('REGISTER_CR1 = ', CR1_INIT())
        # print(readRegister8(REGISTER_CR1))
        self.write_register(self.REGISTER_CR1, self.CR1_INIT())
        # print(readRegister8(REGISTER_CR1))

        self.REGISTER_MASK = 2
        # print('MASK_INIT = ', MASK_INIT())
        # print(readRegister8(REGISTER_MASK))
        self.write_register(self.REGISTER_MASK, self.MASK_INIT())
        # print(readRegister8(REGISTER_MASK))
        pyb.delay(500)

    def READ_OPERATION(self, x):
        return x & 0x7F

    def WRITE_OPERATION(self, x):
        return x | 0x80

    def CR0_INIT(self):
        self.CR0_AUTOMATIC_CONVERSION = 0x80
        self.CR0_OPEN_CIRCUIT_FAULT_TYPE_K = 0x10
        return self.CR0_AUTOMATIC_CONVERSION + self.CR0_OPEN_CIRCUIT_FAULT_TYPE_K

    def CR1_INIT(self):
        self.CR1_AVERAGE_2_SAMPLES = 0x10
        self.CR1_THERMOCOUPLE_TYPE_K = 0x03
        return self.CR1_AVERAGE_2_SAMPLES + self.CR1_THERMOCOUPLE_TYPE_K

    def MASK_INIT(self):
        self.MASK_VOLTAGE_UNDER_OVER_FAULT = 0x02
        self.MASK_THERMOCOUPLE_OPEN_FAULT = 0x01
        return ~(self.MASK_VOLTAGE_UNDER_OVER_FAULT + self.MASK_THERMOCOUPLE_OPEN_FAULT)

    def read_register8(self, addr):
        b = bytearray(1)
        b[0] = self.READ_OPERATION(addr)
        self.cs.low()
        self.spi.send(b)
        b = self.spi.recv(1)
        self.cs.high()
        return int(b[0])

    def read_register16(self, addr):
        b = bytearray(1)
        b[0] = self.READ_OPERATION(addr)
        res = bytearray(2)
        self.cs.low()
        self.spi.send(b)
        res = self.spi.recv(2)
        self.cs.high()
        num = res[0] << 8 | res[1]
        return int(num)

    def read_register24(self, addr):
        b = bytearray(1)
        b[0] = self.READ_OPERATION(addr)
        res = bytearray(3)
        self.cs.low()
        self.spi.send(b)
        res = self.spi.recv(3)
        self.cs.high()
        num = res[0] << 16 | res[1] << 8 | res[0]
        return int(num)

    def write_register(self, addr, data):
        b = bytearray(2)
        b[0] = self.WRITE_OPERATION(addr)
        b[1] = data
        self.cs.low()
        self.spi.send(b)
        self.cs.high()

    def oneshottemp(self):
        self.MAX31856_CJTO_REG = 0x09
        self.write_register(self.MAX31856_CJTO_REG, 0x0)
        self.MAX31856_CR0_REG = 0x00
        t = self.read_register8(self.MAX31856_CR0_REG)
        self.MAX31856_CR0_AUTOCONVERT = 0x80
        t &= ~self.MAX31856_CR0_AUTOCONVERT  # turn off autoconvert!
        self.MAX31856_CR0_1SHOT = 0x40
        t |= self.MAX31856_CR0_1SHOT
        self.write_register(self.MAX31856_CR0_REG, t)
        pyb.delay(250)

    def read_junction(self, unit):
        self.oneshottemp()
        self.MAX31856_CJTH_REG = 0x0A
        temp16 = self.read_register16(self.MAX31856_CJTH_REG)
        temp16 = float(temp16)
        temp16 /= 256.0
        return temp16

    def read_temp(self, unit):
        self.oneshottemp()
        self.MAX31856_LTCBH_REG = 0x0C
        temp24 = self.read_register24(self.MAX31856_LTCBH_REG)
        if temp24 & 0x800000:
            temp24 |= 0xFF000000
        temp24 >>= 5
        temp24 = float(temp24)
        temp24 *= 0.0078125
        return temp24

    def read_fault(self):
        self.MAX31856_SR_REG = 0x0F
        data = self.read_register8(self.MAX31856_SR_REG)
        return data
    
    def get_temperature_outside(self):
        return self.read_fault(), self.read_temp(0)

    def temp_skip(self):
        for i in range(10):
            self.get_temperature_outside()
            pyb.delay(500)
