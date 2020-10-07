#include "Arduino.h"
#include "Modules.h"
#include <stdlib.h>
#include <SPI.h>
#include "SD.h"

#define START 5
#define DRDY 19
#define SCLK 4
#define S1 A9
#define S2 A8
#define S3 A11
#define S4 A10
#define CS 53
#define RE 6
#define DE 7

volatile byte dataReadyFlag;
volatile byte data;

unsigned long signal = 0, sum = 0,sum1 = 0, volt = 0;
int iter = 0, mode = 0;
float buf_rentgen[46], mkrentgen = 0.0;

void count()
{
    signal++;
}

void dataReady()
{
    // do somth
    dataReadyFlag = 1;
    digitalWrite(13, HIGH);
    digitalWrite(13, LOW);
}

void setup()
{
    pinMode(31, OUTPUT);
    attachInterrupt(0, count, RISING);
    attachInterrupt(1, count, RISING);

    pinMode(START, OUTPUT);
    pinMode(DRDY, INPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(S4, OUTPUT);

    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    digitalWrite(S4, HIGH);

    pinMode(CS, OUTPUT);
    SD.begin(CS);

    digitalWrite(START, HIGH);
    digitalWrite(SCLK, LOW);

    dataReadyFlag = 0;

    attachInterrupt(4, dataReady, FALLING);

    memset(buf_rentgen, 0, 46);

}

void loop()
{
   unsigned long start = 0, stop = 0, stop1 = 0, start1 = 0;
   float buf_signal;
   start = millis();
   start1 = millis();
   delay(10);
   while (dataReadyFlag != 1) {;}
   dataReadyFlag = 0;
   noInterrupts();

   delayMicroseconds(5);

   for (volatile int i = 0; i < 25; i++)
   {
       digitalWrite(SCLK, HIGH);
       delayMicroseconds(5);
       digitalWrite(SCLK, LOW);
       delayMicroseconds(5);
       data = digitalRead(DRDY);
       volt |= (data&0x01) << 24-i;
   }
//     magic
   EIFR |= 1 << 2;

   interrupts();
//    log_data(mode, volt);
   volt = 0;
   stop = millis();
   sum = sum + (stop - start);
   start = 0;
   stop = 0;
   if(sum > 500)
   {
       mode++;
       sum = 0;
       mode = changeMode(mode);
   }
   stop1 = millis();
   sum1 = sum1 + (stop1 - start1);
   stop1 = 0;
   start1 = 0;
   if(sum1 > 1000)
   {
       buf_signal = float(signal);
       if(mkrentgen == 0.0)
       {
           if(iter < 46)
           {
               buf_rentgen[iter] = buf_signal;
               signal = 0;
               sum1 = 0;
               log_rad(buf_rentgen[iter]);
//                Serial2.print("BufRad: ");Serial2.println(buf_rentgen[iter]);
           }
           iter++;
           if(iter == 46)
           {
               for (int i = 0; i < 46; ++i)
               {
                   mkrentgen = mkrentgen + buf_rentgen[i];
               }
               mkrentgen = mkrentgen/2.0;
//                Serial2.print("Rad: ");Serial2.println(mkrentgen);
               log_rad(mkrentgen);
               iter = 0;
               signal = 0;
           }
       }
       else
       {
           buf_rentgen[iter] = buf_signal;
           mkrentgen = 0.0;
           for (int i = 0; i < 46; ++i)
           {
               mkrentgen = mkrentgen + buf_rentgen[i];
           }
           mkrentgen = mkrentgen/2.0;
           signal = 0;
           sum1 = 0;
//            Serial2.print("Rad: ");Serial2.println(mkrentgen);
           log_rad(mkrentgen);
           iter++;
           if(iter == 46)
           {
               iter = 0;
           }
       }
   }
}
