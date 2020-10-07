#ifndef CPU_MODULES_H
#define CPU_MODULES_H

#include "Tools.h"

typedef struct Low_frequency_data
{
    unsigned long time;
    GPS GPS_data;
    float rad;
    float alt_pres = 125.0;
}
        Low_frequency_data;

typedef struct Medium_frequency_data
{
    unsigned long time;
    float Temp_in;
    float Pres[2];
    float Bat_volt;
}
        Medium_frequency_data;

typedef struct Fast_frequency_data
{
    Acceleration accel1;
}
        Fast_frequency_data;

typedef struct init_checking
{
    float Pressure;
    float Temperature;
}
        init_checking;

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

void GPS_init();
Low_frequency_data get_low_data(RTC_DS1307);
Medium_frequency_data get_medium_data(RTC_DS1307);
Fast_frequency_data get_fast_data();
void log_low_data(Low_frequency_data, int);
void log_medium_data(Medium_frequency_data, int);
void log_fast_data(Fast_frequency_data, int);
//void NVRAM_log();
void sent_telemetry(Fast_frequency_data, Medium_frequency_data, Low_frequency_data);
init_checking algorhythm_init();
float check_altitude(float, init_checking, float);
void launch_rescue_system(float alt);
void make_clip_checking(float alt);

#endif //CPU_MODULES_H