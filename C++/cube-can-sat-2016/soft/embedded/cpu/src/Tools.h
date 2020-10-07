#ifndef CPU_SENSORS_H
#define CPU_SENSORS_H

#include "RTClib.h"
#define RE 6
#define DE 7

typedef struct Acceleration
{
    float Ax;
    float Ay;
    float Az;
}
        Acceleration;

typedef struct GPS
{
    float lat = 0.0;
    float lon = 0.0;
    float alt = 0.0;
    float HDOP = 0.0;
    float VDOP = 0.0;
    byte sats = 18;
}
        GPS;

typedef struct
{
    char raw[20];
}
        ParseData;

float get_temperature();
Acceleration get_accel(int);
float get_pres(int);
unsigned long get_time(RTC_DS1307);
GPS GPS_parse();
void front_Cam_init();
void makeClip_front_cam();
void topCam_init();
void makeClip_topCam();
float get_volt();
int get_ozone();
float get_rad();

#endif