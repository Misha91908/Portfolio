#ifndef TELEM_TOOLS_H
#define TELEM_TOOLS_H

typedef struct Acceleration
{
    float Ax;
    float Ay;
    float Az;
}
        Acceleration;


typedef struct GPS
{
    float lat;
    float lon;
    float alt;
    float HDOP;
    float VDOP;
    int Sats;
}
        GPS;

typedef union
{
    float value;
    byte bin[4];
}
        binaryFloat;

typedef union
{
    unsigned long value;
    byte bin[4];
}
        binaryULong;

typedef struct parseData
{
    unsigned long time;
    GPS GPS_data;
    float TempIn;
    float TempOut;
    float Pressure[2];
    float BatVolt;
    Acceleration accel;
    float Radiation;
    float Dust;
    float Ozone;
}
        parseData;

void GSM_init();
void sent_GPRS(parseData);
void sendData();
void sentData(parseData);
void gprsconnect();
parseData getData();
void sent_APRS(parseData, int);
void sent_Telemetry(parseData);

#endif //TELEM_TOOLS_H