import pyb


class RunCam:
    def __init__(self, pwm_pin, timer, ch):
        self.state = False
        self.pwm_pin = pwm_pin
        self.timer = timer
        self.max_width = 1332
        self.min_width = 666
        self.tchannel = self.timer.channel(ch, pyb.Timer.PWM, pin=self.pwm_pin, pulse_width=0)
        self.tchannel.pulse_width(self.min_width)
    
    def change_mode_command(self, port, mode, i2c):
        if mode == 0:
            i2c.send(0x00, addr=0x3A)
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
#            port.write(b'\xf0\xbb\x01\x03\xd5\x00\x48\xaf\xfa\xdd')
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).low()
        if mode == 1:
            i2c.send(0x01, addr=0x3A)
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
#            port.write(b'\xf0\xbb\x01\x03\xd5\x01\x88\x6e\xfa\xdd')
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).low()
        if mode == 2:
            i2c.send(0x02, addr=0x3A)
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
#            port.write(b'\xf0\xbb\x01\x03\xd5\x02\x89\x2e\xfa\xdd')
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).low()
        if mode == 3:
            i2c.send(0x03, addr=0x3A)
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).high()
#            port.write(b'\xf0\xbb\x01\x03\xd5\x03\x49\xef\xfa\xdd')
#            pyb.Pin('PE3', pyb.Pin.OUT_PP).low()

    def set_standby(self):
        if self.state:
            self.tchannel.pulse_width(self.min_width)
            self.state = False
        else:
            self.tchannel.pulse_width(self.max_width)
            self.state = True

    def change_mode(self):
        print('Change mode')
        if self.state:
            self.tchannel.pulse_width(self.min_width)
            pyb.delay(500)
            self.tchannel.pulse_width(self.max_width)
            self.state = True
        else:
            self.tchannel.pulse_width(self.max_width)
            pyb.delay(500)
            self.tchannel.pulse_width(self.min_width)
            self.state = False

    def make_photo(self):
        if self.state:
            self.tchannel.pulse_width(self.min_width)
            self.state = False
        else:
            self.tchannel.pulse_width(self.max_width)
            self.state = True
        print('Make photo')

