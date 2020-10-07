#include <Arduino.h>
#include <math.h>
#include "Tools.h"
#include "modules.h"
#include "Servo.h"
#include <SD.h>
#include <SPI.h>

const int MPU_addr = 0x69; // accelerometer i2c address
const int MS_addr1 = 0x76;
const int MS_addr2 = 0x77;

void GPS_init()
{
    byte message[] = {0xB5, 0x62, 0x08, 0x24, 0x24, 0x00, 0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0xBF};
    Serial1.write(message, sizeof(message));
}

Low_frequency_data get_low_data(RTC_DS1307 rtc)
{
    Low_frequency_data packet;
    packet.time = get_time(rtc);
    packet.GPS_data = GPS_parse();
//    packet.rad = get_rad();
//    Serial.println(packet.rad);
    return packet;
}

Medium_frequency_data get_medium_data(RTC_DS1307 rtc)
{
    Medium_frequency_data packet;
    packet.time = get_time(rtc);
    packet.Temp_in = get_temperature();
    packet.Pres[0] = get_pres(MS_addr1);
    delay(5);
    packet.Pres[1] = get_pres(MS_addr2);
    packet.Bat_volt = get_volt();
    Serial.println(packet.Bat_volt);

    return packet;
}

Fast_frequency_data get_fast_data()
{
    Fast_frequency_data packet;
    packet.accel1 = get_accel(MPU_addr);

    return packet;
}

void log_low_data(Low_frequency_data packet, int CS_pin)
{
    File f = SD.open("low.txt", FILE_WRITE);
    if(f)
    {
        f.print(packet.time);f.print(';');
        f.print(packet.GPS_data.lat);f.print(';');
        f.print(packet.GPS_data.lon);f.print(';');
        f.print(packet.GPS_data.alt);f.print(';');
        f.print(packet.GPS_data.HDOP);f.print(';');
        f.println(packet.GPS_data.VDOP);
        f.close();
    }

}

void log_medium_data(Medium_frequency_data packet, int CS_pin)
{
    File f = SD.open("medium.txt", FILE_WRITE);
    if(f)
    {
        f.print(packet.time);f.print(';');
        f.print(packet.Temp_in);f.print(';');
        f.print(packet.Pres[0]);f.print(';');
        f.print(packet.Pres[1]);f.print(';');
        f.println(packet.Bat_volt);
        f.close();
    }

}

void log_fast_data(Fast_frequency_data packet, int CS_pin)
{
    File f = SD.open("fast.txt", FILE_WRITE);
    if(f)
    {
        f.print(packet.accel1.Ax);f.print(';');
        f.print(packet.accel1.Ay);f.print(';');
        f.println(packet.accel1.Az);
        f.close();
    }
}

//void NVRAM_log()
//{
//    write_data(String(packet.time.day)+String(packet.time.hour)+String(packet.time.minute)+String(packet.time.second), 22);
//    write_data(String(packet.temperature), 22);
//    write_data(String(packet.pressure[0]), 22);
//    write_data(String(packet.accel[0].Ax), 22);
//    write_data(String(packet.accel[0].Ay), 22);
//    write_data(String(packet.accel[0].Az), 22);
//    write_data(String(packet.accel[1].Ax), 22);
//    write_data(String(packet.accel[1].Ay), 22);
//    write_data(String(packet.accel[1].Az), 22);
//    Serial.println("Successful writing!");
//}

void sent_telemetry(Fast_frequency_data packet, Medium_frequency_data packet1, Low_frequency_data packet2)
{
    binaryFloat fdata;
    binaryULong uldata;

    Serial3.write(packet2.GPS_data.sats);
    delay(1);
    uldata.value = packet2.time;
    Serial3.write(uldata.bin, 4);
    delay(1);

    fdata.value = packet2.GPS_data.lat;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet2.GPS_data.lon;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet2.GPS_data.alt;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet1.Temp_in;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet1.Pres[0];
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet1.Pres[1];
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet1.Bat_volt;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet.accel1.Ax;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet.accel1.Ay;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet.accel1.Az;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet2.GPS_data.HDOP;
    Serial3.write(fdata.bin, 4);
    delay(1);
    fdata.value = packet2.GPS_data.VDOP;
    Serial3.write(fdata.bin, 4);
    delay(1);
}

init_checking algorhythm_init()
{
    init_checking data;
    data.Temperature = get_temperature();
    delay(10);
    data.Pressure = get_pres(MS_addr1);
    return data;
}

float check_altitude(float alt, init_checking data, float lat)
{
    float difference, P2, temp2;

    P2 = get_pres(MS_addr1);
    temp2 = get_temperature();

    difference = 18336.0 * (1 + 0.002845 * cos(2 * lat * 0.017453)) * (1 + ((data.Temperature + temp2) / 500.0)) *
                 log10(data.Pressure / P2);

    alt += difference;
    Serial.println(alt);

    return alt;
}

void launch_rescue_system(float alt)
{
    if(alt < 650.5)
    {
        digitalWrite(30, LOW);
        delay(250);
        digitalWrite(30, HIGH);
    }
    else
        return;
}

void make_clip_checking(float alt)
{
    for (int i = 1; i < 8; ++i)
    {
        if((alt > (i * 5000.0) - 150.0) && (alt < (i * 5000.0) + 150.0))
        {
            makeClip_front_cam();
        }
    }
}