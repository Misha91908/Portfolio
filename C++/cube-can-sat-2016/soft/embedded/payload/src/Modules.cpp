#include <Arduino.h>
#include <SPI.h>
#include "Modules.h"
#include "SD.h"
#include "Wire.h"

#define S1 A9
#define S2 A8
#define S3 A11
#define S4 A10

int changeMode(int mode)
{
    switch(mode)
    {
        case 1:
            digitalWrite(S1, LOW);
            digitalWrite(S2, HIGH);
            digitalWrite(S3, LOW);
            digitalWrite(S4, HIGH);
            delayMicroseconds(5);
            break;
        case 2:
            digitalWrite(S1, HIGH);
            digitalWrite(S2, LOW);
            digitalWrite(S3, HIGH);
            digitalWrite(S4, LOW);
            delayMicroseconds(5);
            break;
        default:
            mode = 0;
            digitalWrite(S1, LOW);
            digitalWrite(S2, LOW);
            digitalWrite(S3, HIGH);
            digitalWrite(S4, HIGH);
            delayMicroseconds(5);
    }
    return mode;
}

void log_data(int mode, unsigned long volt)
{
    File f = SD.open("LOG.txt", FILE_WRITE);
    if(f)
    {
        f.print(mode);
        f.print(';');
        f.println(volt);
        f.close();
    }

}

void log_rad(float mkrentgen)
{
    File f = SD.open("radiation.txt", FILE_WRITE);
    if(f)
    {
        f.println(mkrentgen);
        f.close();
    }

}