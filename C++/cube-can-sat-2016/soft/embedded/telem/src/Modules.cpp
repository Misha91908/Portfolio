#include <Arduino.h>
#include "Modules.h"
#include <stdlib.h>

#define gsm Serial1
#define APRS Serial3

String response;

void GSM_init()
{
    int start = 1;
//    Serial.println("M590 Modem init start...");
    while (1)
    {
        if (gsm.find("MODEM:STARTUP"))
        {
//            Serial.println("Modem booting...");
            break;
        };
//        Serial.println("Searching for modem...");
        delay(100);
        if(start == 1)
        {
            digitalWrite(31, HIGH);
            start++;
        }
    }

    while (1)
    {
        if (gsm.find("+PBREADY"))
        {
//            Serial.println("Modem booting...ok");
            break;
        };
        delay(100);
    }

    gsm.println("ATE0");
    delay(50);
    response = gsm.readStringUntil("OK");
//    Serial.println(response);

    gsm.println("ATI");
    delay(50);
    response = gsm.readStringUntil("OK");
//    Serial.println(response);

    gsm.println("AT+GETVERS");
    delay(50);
    response = gsm.readStringUntil("OK");
//    Serial.println(response);

    do
    {
        // проверяем готовность модема
        gsm.println("AT+CPAS");
//        Serial.print(".");
        delay(100);
    }
    while(!gsm.find("0"));

    do
    {
        gsm.println("AT+CREG?");
//        Serial.print(":");
        delay(100);
    }
    while(!gsm.find("+CREG: 0,1"));

    gsm.flush();

    gprsconnect();

    gsm.println("AT+XIIC?");
    delay(50);
    response = gsm.readStringUntil("OK");
//    Serial.println(response);

    gsm.flush();

    gsm.println("AT+TCPSETUP=0,212.109.220.21,5100");
    delay(100);
    response = gsm.readStringUntil("OK");
//    Serial.println(response);

    gsm.flush();
}

parseData getData()
{
    parseData packet;
    binaryFloat data[11];
    binaryULong dat;

    while(!Serial.available()){;}

    packet.GPS_data.Sats = Serial.read();
    delay(2);

    for (int i = 0; i < 4; ++i)
    {
        dat.bin[i] = Serial.read();
        delay(2);
    }

    for (int j = 0; j < 11; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            data[j].bin[i] = Serial.read();
            delay(2);
        }
    }

    packet.time = dat.value;

    packet.GPS_data.lat = data[0].value;
    packet.GPS_data.lon = data[1].value;
    packet.GPS_data.alt = data[2].value;
    packet.TempIn = data[3].value;
    packet.Pressure[0] = data[4].value;
    packet.Pressure[1] = data[5].value;
    packet.BatVolt = data[6].value;
    packet.accel.Ax = data[7].value;
    packet.accel.Ay = data[8].value;
    packet.accel.Az = data[9].value;
    packet.GPS_data.HDOP = data[10].value;
    packet.GPS_data.VDOP = data[11].value;

//    Serial.println("Packet has been recieved!");

    return packet;
}

void sent_Telemetry(parseData packet)
{
    APRS.print("!SYT");APRS.print(";");
    APRS.print(packet.GPS_data.Sats);APRS.print(";");
    APRS.print(packet.time);APRS.print(";");
    APRS.print(packet.GPS_data.lat);APRS.print(";");
    APRS.print(packet.GPS_data.lon);APRS.print(";");
    APRS.print(packet.GPS_data.alt);APRS.print(";");
    APRS.print(packet.TempIn);APRS.print(";");
    APRS.print(packet.Pressure[0]);APRS.print(";");
    APRS.print(packet.Pressure[1]);APRS.print(";");
    APRS.print(packet.BatVolt);APRS.print(";");
    APRS.print(packet.accel.Ax);APRS.print(";");
    APRS.print(packet.accel.Ay);APRS.print(";");
    APRS.print(packet.accel.Az);APRS.print(";");
    APRS.print(packet.GPS_data.HDOP);APRS.print(";");
    APRS.print(packet.GPS_data.VDOP);APRS.print(":");
//    Serial.println("Telemetry!");
}

void sent_GPRS(parseData packet)
{
    do
    {
        gsm.println("AT+CREG?");
//        Serial.print(":");
        delay(50);
    }
    while(!gsm.find("+CREG: 0,1"));
    gsm.println("at+tcpsend=0,59");
    sentData(packet);
//    Serial.println("GPRS");
}

void sentData(parseData pac)
{
    byte buf[55];
    String tr;
    binaryULong dat;
    binaryFloat data;

    buf[0] = 'S';
    buf[1] = 'Y';
    buf[2] = 'T';
    buf[3] = pac.GPS_data.Sats;
    dat.value = pac.time;
    buf[4] = dat.bin[0];
    buf[5] = dat.bin[1];
    buf[6] = dat.bin[2];
    buf[7] = dat.bin[3];
    data.value = pac.GPS_data.lat;
    buf[8] = data.bin[0];
    buf[9] = data.bin[1];
    buf[10] = data.bin[2];
    buf[11] = data.bin[3];
    data.value = pac.GPS_data.lon;
    buf[12] = data.bin[0];
    buf[13] = data.bin[1];
    buf[14] = data.bin[2];
    buf[15] = data.bin[3];
    data.value = pac.GPS_data.alt;
    buf[16] = data.bin[0];
    buf[17] = data.bin[1];
    buf[18] = data.bin[2];
    buf[19] = data.bin[3];
    data.value = pac.TempIn;
    buf[20] = data.bin[0];
    buf[21] = data.bin[1];
    buf[22] = data.bin[2];
    buf[23] = data.bin[3];
    data.value = pac.Pressure[0];
    buf[24] = data.bin[0];
    buf[25] = data.bin[1];
    buf[26] = data.bin[2];
    buf[27] = data.bin[3];
    data.value = pac.Pressure[1];
    buf[28] = data.bin[0];
    buf[29] = data.bin[1];
    buf[30] = data.bin[2];
    buf[31] = data.bin[3];
    data.value = pac.BatVolt;
    buf[32] = data.bin[0];
    buf[33] = data.bin[1];
    buf[34] = data.bin[2];
    buf[35] = data.bin[3];
    data.value = pac.accel.Ax;
    buf[36] = data.bin[0];
    buf[37] = data.bin[1];
    buf[38] = data.bin[2];
    buf[39] = data.bin[3];
    data.value = pac.accel.Ay;
    buf[40] = data.bin[0];
    buf[41] = data.bin[1];
    buf[42] = data.bin[2];
    buf[43] = data.bin[3];
    data.value = pac.accel.Az;
    buf[44] = data.bin[0];
    buf[45] = data.bin[1];
    buf[46] = data.bin[2];
    buf[47] = data.bin[3];
    data.value = pac.GPS_data.HDOP;
    buf[48] = data.bin[0];
    buf[49] = data.bin[1];
    buf[50] = data.bin[2];
    buf[51] = data.bin[3];
    data.value = pac.GPS_data.VDOP;
    buf[52] = data.bin[0];
    buf[53] = data.bin[1];
    buf[54] = data.bin[2];
    buf[55] = data.bin[3];
    buf[56] = 'T';
    buf[57] = 'Y';
    buf[58] = 'S';

    gsm.write(buf, 55);
//    Serial.print("Ready!");
}

void gprsconnect()
{
    gsm.println("AT+XISP=0");  // включаем РРР
    delay(100);
    gsm.println("AT+CGDCONT=1,\"IP\",\"internet\"");
    delay(100);
    gsm.println("AT+XGAUTH=1,1,\"gdata\",\"gdata\"");
    delay(100);
//  gsm.println("AT+CUSD=1,\"*99***1#\",15");
//  delay(100);
    gsm.println("AT+XIIC=1");
    delay(100);
    do
    {
        // ждем IP
        gsm.println("AT+XIIC?");
//        Serial.print(">");
        delay(200);
    }
    while(gsm.find("0.0.0.0"));
}

void sent_APRS(parseData packet, int id)
{
    int altitude;
    int k = 1;
    APRS.print("!T#");APRS.print(id);
    APRS.print(",");APRS.print(packet.GPS_data.lat);
    APRS.print(",");APRS.print(packet.GPS_data.lon);
    APRS.print(",");APRS.print(packet.GPS_data.alt);
    APRS.print(",");APRS.print(packet.TempOut);
    APRS.print(",");APRS.print(packet.Pressure[0]);
    APRS.print(",");APRS.print("00000000");
//    Serial.println("Send completed!");
    delay(4000);
    if((packet.GPS_data.lat == 0.0) && (packet.GPS_data.lon == 0.0) && (packet.GPS_data.alt == 0.0))
    {
        APRS.print("!=0000.0 N/00000.0 EO/A=000000 SpaceYPNZ");
    }
    else
    {
        APRS.print("!=");APRS.print(int(packet.GPS_data.lat));APRS.print(int((packet.GPS_data.lat - float(int(packet.GPS_data.lat))) * 60.0));APRS.print(".0");
        APRS.print(" N/");
        if(packet.GPS_data.lon < 100.0)
        {
            APRS.print("0");
            APRS.print(int(packet.GPS_data.lon));APRS.print(int((packet.GPS_data.lon - float(int(packet.GPS_data.lon))) * 60.0));
            APRS.print(".0");
        }
        else
        {
            APRS.print(int(packet.GPS_data.lon));APRS.print(int((packet.GPS_data.lon - float(int(packet.GPS_data.lon))) * 60.0));
            APRS.print(".0");
        }
        APRS.print(" EO/A=");
        altitude = int(packet.GPS_data.alt * 3.281);
        while(altitude % 10 != 0)
        {
            k++;
            altitude = altitude / 10;
        }
        for (int i = 0; i < 6-k; ++i)
        {
            APRS.print("0");
        }
        APRS.print(altitude);
        k = 1;
        APRS.print("SpaceYPNZ");
    }
}