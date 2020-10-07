#include "Arduino.h"
#include "Modules.h"
#define gsm Serial1
#define RE 6
#define DE 7
#define APRS Serial3

parseData packet;
int id = 100;
unsigned long sum = 0, sum1 = 0, sum2 = 0;

void setup()
{
    Serial.begin(9600);
    Serial2.begin(9600);
    APRS.begin(9600);
    gsm.begin(57600);

    pinMode(31, OUTPUT);
    pinMode(RE, OUTPUT);
    pinMode(DE, OUTPUT);
    digitalWrite(RE, LOW);
    digitalWrite(DE, LOW);

    pinMode(33, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);

//    GSM_init();
//    Serial.println("Ready");
}

void loop()
{
    unsigned long start = 0, start1 = 0, start2 = 0,stop = 0, stop1 = 0, stop2 = 0;
    start = millis();
//    start1 = millis();
    start2 = millis();
    packet = getData();
    delay(500);
    stop = millis();
    sum = sum + (stop - start);
    start = 0;
    stop = 0;
    if(sum > 10000)
    {
        for (int i = 0; i < 2; ++i)
        {
            digitalWrite(35, HIGH);
            delay(100);
            digitalWrite(35, LOW);
            delay(350);
        }
        digitalWrite(33, HIGH);
        delay(100);
        digitalWrite(33, LOW);
        delay(1000);
        sent_Telemetry(packet);
        delay(3000);
        sum = 0;
    }
//    stop1 = millis();
//    sum1 = sum1 + (stop1 - start1);
//    start1 = 0;
//    stop1 = 0;
//    if(sum1 > 30000)
//    {
//        sent_GPRS(packet);
//        delay(1000);
//        sum1 = 0;
//    }
    stop2 = millis();
    sum2 = sum2 + (stop2 - start2);
    start2 = 0;
    stop2 = 0;
    if(sum2 > 60000)
    {
        if(id % 5 == 0)
        {
            for (int i = 0; i < 3; ++i)
            {
                digitalWrite(35, HIGH);
                delay(100);
                digitalWrite(35, LOW);
                delay(350);
            }
            delay(1000);
            sent_APRS(packet, id);
            delay(3000);
//            Serial.println("APRS0");
        }
        else
        {
            for (int i = 0; i < 2; ++i)
            {
                digitalWrite(35, HIGH);
                delay(100);
                digitalWrite(35, LOW);
                delay(350);
            }
            digitalWrite(34, HIGH);
            delay(100);
            digitalWrite(34, LOW);
            delay(1000);
            sent_APRS(packet, id);
            delay(3000);
//            Serial.println("APRS1");
        }
        id++;
        sum2 = 0;
    }
}