#define P_16 0xA001

static unsigned short crc_tab16[256];

unsigned long counter = 0;
unsigned short start = 0xF1BB;
unsigned short command = 0xA0F1;
unsigned short stopbyte = 0xFADD;
byte length = 0x3D;
unsigned long time = 12644895;

typedef union 
{
  float value;
  byte bin[4];
}
  binaryFloat;

typedef union 
{
  unsigned short value;
  byte bin[2];
}
  binaryUShort;

typedef union 
{
  unsigned long value;
  byte bin[4];
}
  binaryULong;


void setup()
{
  Serial.begin(9600);
  binaryUShort checksum;
  binaryUShort param[3];
  binaryULong dataULong[2];
  binaryFloat dataFloat[13];
  byte buf[64];
}

void init_crc16_tab()
{
  unsigned int i, j;
  unsigned short crc, c;
  for (i = 0; i < 256; i++)
  {
    crc = 0;
    c = i;
    for (j = 0; j < 8; j++)
    {
      if ((crc ^ c) & 0x0001)
      {
        crc = (crc >> 1) ^ P_16;
      }
      else
      {
        crc = crc >> 1;
      }
    c = c >> 1;
    }
     crc_tab16[i] = crc;
  }
}

unsigned short update_crc_16(unsigned short crc, byte c)
{

  unsigned short tmp, short_c;
  short_c = 0x00ff & (unsigned short)c;
   
  tmp = crc ^ short_c;
  crc = (crc >> 8) ^ crc_tab16[tmp & 0xff];
  
  return crc;
}

unsigned long reverse_small_data(unsigned long value)
{
  unsigned long output = 0;
  for(int i = 1; i > -1; i--)
  {
    output += (value & 0xFF) << (8 * i);
    value >>= 8;
  }
  return output;
}

void loop()
{
  randomSeed(counter);
  unsigned short check = 0xFFFF;
  param[0].value = start;
  param[1].value = command;
  param[2].value = stopbyte;
  for(int i = 0; i < 3; i++)
  {
    param[i].value = reverse_small_data(param[i].value);
  }
  buf[0] = param[1].bin[0];
  buf[1] = param[1].bin[1];
  buf[2] = length;

  for(int i = 0; i < 13; i++)
  {
    dataFloat[i].value = float(random(100))/(i + 1);
  }
  byte sat = 12;
  dataULong[0].value = counter;
  dataULong[1].value = time;
  
  // Govnocode generation BEGIN
  buf[3] = dataULong[0].bin[0];
  buf[4] = dataULong[0].bin[1];
  buf[5] = dataULong[0].bin[2];
  buf[6] = dataULong[0].bin[3];
  buf[7] = sat;
  buf[8] = dataULong[1].bin[0];
  buf[9] = dataULong[1].bin[1];
  buf[10] = dataULong[1].bin[2];
  buf[11] = dataULong[1].bin[3];
  buf[12] = dataFloat[0].bin[0];
  buf[13] = dataFloat[0].bin[1];
  buf[14] = dataFloat[0].bin[2];
  buf[15] = dataFloat[0].bin[3];
  buf[16] = dataFloat[1].bin[0];
  buf[17] = dataFloat[1].bin[1];
  buf[18] = dataFloat[1].bin[2];
  buf[19] = dataFloat[1].bin[3];
  buf[20] = dataFloat[2].bin[0];
  buf[21] = dataFloat[2].bin[1];
  buf[22] = dataFloat[2].bin[2];
  buf[23] = dataFloat[2].bin[3];
  buf[24] = dataFloat[3].bin[0];
  buf[25] = dataFloat[3].bin[1];
  buf[26] = dataFloat[3].bin[2];
  buf[27] = dataFloat[3].bin[3];
  buf[28] = dataFloat[4].bin[0];
  buf[29] = dataFloat[4].bin[1];
  buf[30] = dataFloat[4].bin[2];
  buf[31] = dataFloat[4].bin[3];
  buf[32] = dataFloat[5].bin[0];
  buf[33] = dataFloat[5].bin[1];
  buf[34] = dataFloat[5].bin[2];
  buf[35] = dataFloat[5].bin[3];
  buf[36] = dataFloat[6].bin[0];
  buf[37] = dataFloat[6].bin[1];
  buf[38] = dataFloat[6].bin[2];
  buf[39] = dataFloat[6].bin[3];
  buf[40] = dataFloat[7].bin[0];
  buf[41] = dataFloat[7].bin[1];
  buf[42] = dataFloat[7].bin[2];
  buf[43] = dataFloat[7].bin[3];
  buf[44] = dataFloat[8].bin[0];
  buf[45] = dataFloat[8].bin[1];
  buf[46] = dataFloat[8].bin[2];
  buf[47] = dataFloat[8].bin[3];
  buf[48] = dataFloat[9].bin[0];
  buf[49] = dataFloat[9].bin[1];
  buf[50] = dataFloat[9].bin[2];
  buf[51] = dataFloat[9].bin[3];
  buf[52] = dataFloat[10].bin[0];
  buf[53] = dataFloat[10].bin[1];
  buf[54] = dataFloat[10].bin[2];
  buf[55] = dataFloat[10].bin[3];
  buf[56] = dataFloat[11].bin[0];
  buf[57] = dataFloat[11].bin[1];
  buf[58] = dataFloat[11].bin[2];
  buf[59] = dataFloat[11].bin[3];
  buf[60] = dataFloat[12].bin[0];
  buf[61] = dataFloat[12].bin[1];
  buf[62] = dataFloat[12].bin[2];
  buf[63] = dataFloat[12].bin[3];
  // Govnocode generation END
 
  init_crc16_tab();

  for (int i = 0; i < sizeof(buf); i++)
  {
      check = update_crc_16(check, buf[i]);
  }
  
  checksum.value = check;

  Serial.write(param[0].bin, 2);
  Serial.write(param[1].bin, 2);
  Serial.write(length);
  Serial.write(dataULong[0].bin, 4);
  counter++;
  time++;
  Serial.write(sat);
  Serial.write(dataULong[1].bin, 4);
  for(int i = 0; i < 13; i++)
  {
    Serial.write(dataFloat[i].bin, 4);
  }
  Serial.write(checksum.bin, 2);
  Serial.write(param[2].bin, 2);
  delay(1000);
}

