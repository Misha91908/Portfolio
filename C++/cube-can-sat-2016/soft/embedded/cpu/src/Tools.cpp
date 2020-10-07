#include <Arduino.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include "MS5611.h"
#include "OneWire.h"
#include "RTClib.h"
#include <stdlib.h>
#include <string.h>
#include "Tools.h"
#include "Servo.h"
#include "modules.h"
#include <stdint.h>

Servo servoPan;
float servoMin = 0, servoMax = 180;

unsigned long get_time(RTC_DS1307 rtc)
{
    unsigned long data;
    DateTime now = rtc.now();
    data = now.unixtime();
    return data;
}

GPS GPS_parse()
{
    GPS pac;
    ParseData data[3], *dat;
    char buf[500], check_buf[500], *token;
    int k = 0, n = 0;
    size_t tok_count = 0;
    float deg, min;
    String port, log[3];

    memset(buf, 0, 300);
    memset(check_buf, 0, 500);
    memset(data[0].raw, 0, 20);
    memset(data[1].raw, 0, 20);
    memset(data[2].raw, 0, 20);

    while (k < 3)
    {
        port = Serial1.readStringUntil('\n');
        if((port.indexOf("$GNGLL") != -1) && (log[0].length() == 0))
        {
            log[0] = port.substring(0);
            k++;
        }
        if((port.indexOf("$GNGSA") != -1) && (log[1].length() == 0))
        {
            log[1] = port.substring(0);
            k++;
        }
        if((port.indexOf("$GNGGA") != -1) && (log[2].length() == 0))
        {
            log[2] = port.substring(0);
            k++;
        }
    }
    k = 0;

    while (k < log[0].length())
    {
        buf[k] = log[0][k];
        k++;
    }
    k = 0;

    if (strstr(buf, "$GNGLL") != NULL)
    {
        sprintf(check_buf, "%s", buf);
        for (int i = 0; i < strlen(check_buf) - 2; ++i)
        {
            if ((check_buf[i] == ',') && (check_buf[i + 1] == ','))
                k++;
        }
        token = strtok(check_buf,",");
        tok_count++;
        while(token != NULL)
        {
            token = strtok(NULL, ",");
            tok_count++;
        }
        if((tok_count < 3) || (k > 3))
        {
            pac.lat = 0;
            pac.lon = 0;
            memset(buf, 0, 500);
            memset(check_buf, 0, 500);
            k = 0;
        }
        else
        {
            tok_count = 0;
            memset(check_buf, 0, 500);
            k = 0;

            token = strtok(buf, ",");
            while (token != NULL && k < 3)
            {
                token = strtok(NULL, ",");
                size_t p = strlen(token);
                for (int i = 0; i < p; ++i)
                {
                    data[k].raw[i] = token[i];
                }
                k++;
            }
            memset(buf, 0, 500);

            k = 0;
            for (int i = 0; i < strlen(data[0].raw) - 1; ++i)
            {
                if (data[0].raw[0] == '0')
                {
                    if (data[0].raw[i] == '0')
                    {
                        n++;
                        if (data[0].raw[i + 1] != '0')
                            break;
                    }
                }
                else
                    break;
            }
            for (int i = 0; i < strlen(data[0].raw) - n; ++i)
            {
                data[0].raw[i] = data[0].raw[i + n];
            }
            memset(buf, 0, 500);
            buf[0] = data[0].raw[0];
            buf[1] = data[0].raw[1];
            for (int i = 0; i < strlen(data[0].raw) - 2; ++i)
            {
                data[0].raw[i] = data[0].raw[i + 2];
            }
            deg = atof(buf);
            min = atof(data[0].raw) * (10.0 / 6.0);
            pac.lat = deg + (min / 100);

            k = 0;
            n = 0;
            for (int i = 0; i < strlen(data[2].raw) - 1; ++i)
            {
                if (data[2].raw[0] == '0')
                {
                    if (data[2].raw[i] == '0')
                    {
                        n++;
                        if (data[2].raw[i + 1] != '0')
                            break;
                    }
                }
                else
                    break;
            }
            for (int i = 0; i < strlen(data[2].raw) - n; ++i)
            {
                data[2].raw[i] = data[2].raw[i + n];
            }
            memset(buf, 0, 500);
            buf[0] = data[2].raw[0];
            buf[1] = data[2].raw[1];
            for (int i = 0; i < strlen(data[2].raw) - 2; ++i)
            {
                data[2].raw[i] = data[2].raw[i + 2];
            }
            deg = atof(buf);
            min = atof(data[2].raw) * (10.0 / 6.0);
            pac.lon = deg + (min / 100);
            memset(buf, 0, 500);
        }
    }

    while (k < log[1].length())
    {
        buf[k] = log[1][k];
        k++;
    }
    k = 0;
    if (strstr(buf, "$GNGSA") != NULL)
    {
        sprintf(check_buf, "%s", buf);
        for (int i = 0; i < strlen(check_buf) - 2; ++i)
        {
            if ((check_buf[i] == ',') && (check_buf[i + 1] == ','))
                k++;
        }
        token = strtok(check_buf, ",");
        while (token != NULL)
        {
            token = strtok(NULL, ",");
            tok_count++;
        }
        if((tok_count < 5) || (k > 9))
        {
            pac.VDOP = 0;
            pac.HDOP = 0;
            pac.sats = 0;
            tok_count = 0;
            memset(check_buf, 0, 500);
            memset(buf, 0, 500);
            k = 0;
        }
        else
        {
            k = 0;
            memset(check_buf, 0, 500);
            dat = (ParseData *) malloc(tok_count * sizeof(ParseData));

            token = strtok(buf, ",");
            sprintf(dat[0].raw, "%s", token);
            for (int i = 1; i < tok_count; ++i)
            {
                token = strtok(NULL, ",");
                sprintf(dat[i].raw, "%s", token);
            }
            pac.HDOP = atof(dat[tok_count - 2].raw);
            pac.VDOP = atof(dat[tok_count - 1].raw);
            pac.sats = tok_count - 6;
            memset(buf, 0, 500);
            tok_count = 0;
            free(dat);
        }
    }

    while (k < log[2].length())
    {
        buf[k] = log[2][k];
        k++;
    }
    k = 0;
    if (strstr(buf, "$GNGGA") != NULL)
    {
        sprintf(check_buf, "%s", buf);
        for (int i = 0; i < strlen(check_buf) - 2; ++i)
        {
            if ((check_buf[i] == ',') && (check_buf[i + 1] == ','))
                k++;
        }
        token = strtok(check_buf, ",");
        while (token != NULL)
        {
            token = strtok(NULL, ",");
            tok_count++;
        }
        if((tok_count < 5) || (k > 3))
        {
            pac.alt = 0;
            tok_count = 0;
            memset(buf, 0, 500);
            memset(check_buf, 0, 500);
            k = 0;
        }
        else
        {
            memset(check_buf, 0, 500);
            k = 0;
            dat = (ParseData *) malloc(tok_count * sizeof(ParseData));

            token = strtok(buf, ",");
            sprintf(dat[0].raw, "%s", token);
            for (int i = 1; i < tok_count; ++i)
            {
                token = strtok(NULL, ",");
                sprintf(dat[i].raw, "%s", token);
            }
            pac.alt = atof(dat[tok_count - 5].raw);
            memset(buf, 0, 500);
            tok_count = 0;
            free(dat);
        }
    }
    return pac;
}

Acceleration get_accel(int MPU_addr)
{
    Acceleration data;

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission();
    Wire.requestFrom(MPU_addr,6,true);
    data.Ax = Wire.read()<<8|Wire.read();
    data.Ay = Wire.read()<<8|Wire.read();
    data.Az = Wire.read()<<8|Wire.read();
    Wire.end();
    return data;
}

float get_pres(int address)
{
    MS5611 MS5611(address);

    MS5611.init();
    delay(2);
    int result = MS5611.read();
    if (result != 0)
    {
        Serial.println(result);
        return result;
    }
    else
    {
        float data = MS5611.getPressure() * 0.001;
        return data;
    }
}

float get_temperature()
{
    OneWire ds(12);  // on pin 12 (a 4.7K resistor is necessary)

    byte i;
    byte present = 0;
    byte type_s;
    byte data[12];
    byte addr[8];
    float celsius;

    if (!ds.search(addr))
    {
        ds.reset_search();
        return;
    }

    delay(2);

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1);        // start conversion, with parasite power on at the end

    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE);         // Read Scratchpad

    for (i = 0; i < 9; i++)
    {           // we need 9 bytes
        data[i] = ds.read();
    }

    // Convert the data to actual temperature
    // because the result is a 16 bit signed integer, it should
    // be stored to an "int16_t" type, which is always 16 bits
    // even when compiled on a 32 bit processor.
    int16_t raw = (data[1] << 8) | data[0];
    if (type_s)
    {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10)
        {
            // "count remain" gives full 12 bit resolution
            raw = (raw & 0xFFF0) + 12 - data[6];
        }
    }
    else
    {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        //// default is 12 bit resolution, 750 ms conversion time
    }
    celsius = (float) raw / 16.0;
    return celsius;
}


float get_volt()
{
    float volt = 0.0;
    Wire.requestFrom(0x03, 1);
    if(Wire.available() > 0)
    {
        while(Wire.available())
        {
            byte data = Wire.read();
            volt = (1.1 / 255.0) * data * 11;
        }
    }
    return volt;
}

int get_ozone()
{
    int ozone;
    Wire.requestFrom(0x01, 2);
    if(Wire.available() > 0)
    {
        while(Wire.available())
        {
            ozone = Wire.read()<<8|Wire.read();
        }
    }
    return ozone;
}

float get_rad()
{
    binaryFloat rad;
    int i = 0;
    Wire.requestFrom(0x04, 4);
    if(Wire.available() > 0)
    {
        while(Wire.available())
        {
            rad.bin[i] = Wire.read();
            i++;
        }
    }
        return rad.value;
}

void front_Cam_init()
{
    servoPan.attach(23);
    servoPan.write(servoMax);
    delay(5000);
    servoPan.write(servoMin);
    delay(750);
    servoPan.write(servoMax);
}

void makeClip_front_cam()
{
    servoPan.attach(23);
    servoPan.write(servoMax);
}

void topCam_init()
{
    digitalWrite(41, HIGH);
    delay(3000);
    digitalWrite(41, LOW);
    delay(700);
    digitalWrite(40, HIGH);
    delay(150);
    digitalWrite(40, LOW);
    delay(450);
    digitalWrite(40, HIGH);
    delay(150);
    digitalWrite(40, LOW);
}

void makeClip_topCam()
{
    digitalWrite(41, HIGH);
    delay(150);
    digitalWrite(41, LOW);
}