import pyb


class GPS:
    def __init__(self, port):
        self.port = port
        self.port.init(9600, timeout=50)
        self.answer = b''
        self.lat = 0.0
        self.lon = 0.0
        self.alt = 0.0
        self.hdop = 0.0
        self.vdop = 0.0
        self.sats = 0

    def configure(self):
        airborne_status = self.set_airborne_mode()
        pyb.delay(3000)
        self.port.write(b'$PUBX,40,GSV,0,0,0,0,0,0*59\r\n')
        pyb.delay(3000)
        self.port.write(b'$PUBX,40,RMC,0,0,0,0,0,0*47\r\n')
        pyb.delay(3000)
        self.port.write(b'$PUBX,40,VTG,0,0,0,0,0,0*5E\r\n')
        pyb.delay(3000)
        self.port.write(b'$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n')
        pyb.delay(3000)
        self.port.write(b'\xB5\x62\x06\x08\x06\x00\x65\x00\x01\x00\x01\x00\x7B\x18')
        ack_packet = [0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, 0x06, 0x08, 0, 0]

        ack_byte_id = 0

        for i in range(2, 8):
            ack_packet[8] = ack_packet[8] + ack_packet[i]
            ack_packet[9] = ack_packet[9] + ack_packet[8]
        print('Start getting UBX ACK packet request')

        ack_bytes = bytes(ack_packet)
        counter = 0
        while self.port.read(1) != b'\xb5':
            counter += 1
            print('Trying to read:', counter)
            if counter > 1000:
                break
        else:
            self.answer = b'\xb5' + self.port.read(9)
        counter = 0

    def set_airborne_mode(self):

        gps_set_success = False
        message = bytes([0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06,
                         0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
                         0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C,
                         0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC])
        print('Start airborne initialization...')

        self.sendubx(message)
        gps_set_success = self.getubx_ack(list(message))
        print('Status setup mode:', gps_set_success)
        return gps_set_success

    def sendubx(self, message):
        self.port.write(b'\xff')
        pyb.delay(500)
        self.port.write(message)
        print('Succsessful sending config')

    def getubx_ack(self, message):
        print('Start getting UBX ACK packet request')
        start = pyb.millis()
        ack_packet = [0xB5, 0x62, 0x05, 0x01, 0x02, 0x00, message[2], message[3], 0, 0]

        ack_byte_id = 0

        for i in range(2, 8):
            ack_packet[8] = ack_packet[8] + ack_packet[i]
            ack_packet[9] = ack_packet[9] + ack_packet[8]

        ack_bytes = bytes(ack_packet)
        counter = 0
        while self.port.read(1) != b'\xb5':
            counter += 1
            print('Trying to read:', counter)
            if counter > 1000:
                break
        else:
            self.answer = b'\xb5' + self.port.read(9)
        counter = 0
        if ack_bytes == self.answer:
            return True
        else:
            return False

    def parse(self):
        gga = ''
        gsa = ''
        count = 0
        if self.port.any() > 0:
            while count != 2:
                byteline = self.port.readline()
                if str(byteline) != "b''":
                    line = str(byteline).split("'")
                else:
                    break
                if len(line) > 1:
                    line = line[1]
                else:
                    break
                if line.find('GGA') != -1 and gga == '' and len(line.split(',')) == 15:
                    gga = line
                    count += 1
                if line.find('GSA') != -1 and gsa == '' and len(line.split(',')) == 18:
                    gsa = line
                    count += 1
            if gga and gsa:
                print(gga)
                print(gsa)
                gga = gga.split(',')
                gsa = gsa.split(',')
                try:
                    self.lat = (float(gga[2]) // 100) + ((float(gga[2]) / 100) - (float(gga[2]) // 100)) if gga[2] else 0.0
                except Exception:
                    self.lat = 0.0
                try:
                    self.lon = (float(gga[4]) // 100) + ((float(gga[4]) / 100) - (float(gga[4]) // 100)) if gga[4] else 0.0
                except Exception:
                    self.lon = 0.0
                try:
                    self.alt = float(gga[9]) if gga[9] else 0.0
                except Exception:
                    self.alt = 0.0
                try:
                    self.hdop = float(gsa[len(gsa) - 2]) if gsa[len(gsa) - 2] else 0.0
                except Exception:
                    self.hdop = 0.0
                try:
                    self.vdop = float(gsa[len(gsa) - 1].split('*')[0]) if gsa[len(gsa) - 1] else 0.0
                except Exception:
                    self.vdop = 0.0
                self.sats = 0
                try:
                    if len(gsa) > 7:
                        for i in range(3, 14):
                            if gsa[i]:
                                self.sats += 1
                except Exception:
                    self.sats = 0
            else:
                self.lat = 0.0
                self.lon = 0.0
                self.alt = 0.0
                self.hdop = 0.0
                self.vdop = 0.0
                self.sats = 0

