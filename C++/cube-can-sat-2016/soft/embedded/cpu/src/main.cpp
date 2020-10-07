#include <Arduino.h>
#include "Tools.h"
#include "RTClib.h"
#include "modules.h"
#include "Wire.h"
#include <SD.h>
#include <SPI.h>
#define RE 6
#define DE 7

RTC_DS1307 rtc;

const uint8_t SD_CS = 8;   // chip select for sd
Low_frequency_data packet2;
Medium_frequency_data packet1;
Fast_frequency_data packet;
init_checking begin;
int point = 0;
unsigned long start, start1, stop, stop1, start2, stop2, sum = 0, sum1 = 0, sum2 = 0;

void setup(void)
{
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);
    Serial3.begin(9600);
    Wire.begin();
    rtc.begin();
    GPS_init();
    delay(20000);
//    rtc.adjust(DateTime(2017, 6, 29, 20, 43, 0));
    front_Cam_init();
    delay(3000);
    pinMode(RE,OUTPUT);
    pinMode(DE,OUTPUT);
    pinMode(30, OUTPUT);
    pinMode(41, OUTPUT);
    pinMode(40, OUTPUT);
    topCam_init();
    pinMode(SD_CS, OUTPUT);
    SD.begin(SD_CS);
    digitalWrite(30,HIGH);
    digitalWrite(RE,LOW);
    digitalWrite(DE,LOW);
    begin = algorhythm_init();
//    makeClip_front_cam();
}

void loop(void)
{
    start = millis();
    start1 = millis();
    start2 = millis();
    packet = get_fast_data();
    delay(150);
    log_fast_data(packet,SD_CS);
    Serial.println("Fast Data!");
    stop = millis();
    sum = sum + (stop - start);
    stop = 0;
    start = 0;
    if (sum > 1000)
    {
        packet1 = get_medium_data(rtc);
        delay(100);
        log_medium_data(packet1, SD_CS);
        Serial.println("Medium Data!");
        sum = 0;
    }
    stop1 = millis();
    sum1 = sum1 + (stop1 - start1);
    start1 = 0;
    stop1 = 0;
    if(sum1 > 5000)
    {
        packet2 = get_low_data(rtc);
        delay(100);
        log_low_data(packet2, SD_CS);
        sent_telemetry(packet, packet1, packet2);
        float check = check_altitude(packet2.alt_pres, begin, packet2.GPS_data.lat);
        make_clip_checking(check);
        if(check > 15000.0)
        {
            launch_rescue_system(check);
            point++;
        }
        else
        {
            if(point > 0)
                launch_rescue_system(check);
        }
        Serial.println("Long Data!");
        sum1 = 0;
    }
    stop2 = millis();
    sum2 = sum2 + (stop2 - start2);
    stop2 = 0;
    start2 = 0;
    if(sum2 > 25500)
    {
        makeClip_topCam();
        sum2 = 0;
    }
}