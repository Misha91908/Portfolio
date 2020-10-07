# CFG-RATE (0x06,0x08) packet (without header 0xB5,0x62)
# payload length - 6, update rate 200ms, cycles - 1, reference - UTC (0)
packet = [0x06,0x08,0x06, 0x00,0x65, 0x00,0x01, 0x00,0x01, 0x00]

CK_A,CK_B = 0, 0
for i in range(len(packet)):
  CK_A = CK_A + packet[i]
  CK_B = CK_B + CK_A

# ensure unsigned byte range
CK_A = CK_A & 0xFF
CK_B = CK_B & 0xFF

print ("UBX packet checksum:", CK_A, CK_B)


