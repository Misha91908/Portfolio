int blink_led = 13;

void setup() {
  pinMode(blink_led, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {;}
}

unsigned char wtf[] = {
  0xf1,0xa0,            //header
  0xed,0x00,0x00,0x00,  //packet ID
  0x00,                 //num of sats
  0xd3,0x48,0x88,0x5a,  //timestamp
  0x00,0x80,0xf8,0x41,  //temp_in
  0x8f,0xc2,0xbb,0x41,  //temp_out
  0x59,0x79,0xca,0x42,  //pressure
  0x00,0x48,0x80,0xc6,  //sccel_x
  0x00,0x80,0x92,0x44,  //accel_y
  0x00,0x80,0xc7,0xc4,  //accel_z
  0x00,0x00,0x00,0x00,  //lat
  0x00,0x00,0x00,0x00,  //lon
  0x00,0x00,0x00,0x00,  //alt
  0x00,0x00,0x00,0x00,  //HDOP
  0x00,0x00,0x00,0x00,  //VDOP
  0x7b,0x14,0x02,0x41,  //batt voltage
  0xa4,0x70,0x3d,0x3f,  //batt current
  0x42,                 //checksum?
  0xfa                  //stop
  };

int i = 0;
unsigned long id = 0;
unsigned long time = 1519061770;

typedef union {
       double dbl;
       unsigned char bytes[sizeof(double)];
} float_bytes;

float_bytes crc;

unsigned char packet_len = 65;

void loop() {
  digitalWrite(13, HIGH);

  memcpy(wtf+7, &time, 4);
  memcpy(wtf+2, &id, 4);
  
  id++;
  time += random(1,3);

  crc.dbl = 0.0;
  for(i=2; i<packet_len-2; i++){
    crc.dbl += wtf[i];
  }
  crc.dbl = crc.dbl / (packet_len-4);
  
  crc.bytes[0] ^= crc.bytes[1]^crc.bytes[2]^crc.bytes[3];

  for(i=0; i<packet_len; i++){
    Serial.write(wtf[i]);
  }

  digitalWrite(13, LOW);
  delay(100);
}
