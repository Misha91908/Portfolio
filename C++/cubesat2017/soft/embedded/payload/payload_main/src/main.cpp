#include <Wire.h>
#include <avr/io.h>
#include "atmega-timers.h"
#include "Modules.h"
#include <stdlib.h>
#include "SPI.h"
#include "SD.h"
#include "DS3231.h"
#include "MS5611.h"
#define STOP_TIMER0 TCCR0B &= 0B11111000
#define BOARD_TEMP A9
#define BAR_TEMP A10
int i = 0;

MS5611 bar(0x77);
MS5611 bar1(0x76);

DS3231 clock;
RTCDateTime dt;
unsigned long signal = 0, start = 0, stop = 0, sum = 0, b = 0;
unsigned int iter = 0, buf_signal = 0;
float buf_rentgen[46], mkrentgen = 0.0;
int CS = 53;
PayloadData container;

void configure()
{
    UCSR0B = 0B00011110;
}

void count()
{
    signal++;
}

void inter()
{
    i++;
    if (i == 78)
    {
        digitalWrite(13, HIGH);
    }
    else if (i == 156)
    {
        digitalWrite(13, LOW);
        i = 0;
    }
}

void setup()
{
    memset(buf_rentgen, 0, 46);

    Wire.begin();

    pinMode(2, INPUT);
    pinMode(3, INPUT);
    attachInterrupt(0, count, RISING);
    attachInterrupt(1, count, RISING);

    analogReference(INTERNAL2V56);
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    Serial.begin(9600);
    Serial1.begin(115200);
    Serial.println("payload launch");
    bar.init();

    pinMode(CS, OUTPUT);
    pinMode(11, OUTPUT);
    digitalWrite(11, LOW);
    if (!SD.begin(CS)) {
        Serial.println("Card failed, or not present");
        while (1);
    }
    Serial.println("card initialized.");

    clock.begin();
    clock.armAlarm1(false);
    clock.clearAlarm1();

//    clock.setDateTime(2018, 6, 30, 19, 1, 0);
    clock.setAlarm1(0, 0, 0, 0, DS3231_EVERY_SECOND);

    cli();
    STOP_TIMER0;
    configure();
    sei();
    timer1(TIMER1_PRESCALER_1024, 1000U, inter);
}

void loop() {
    if (clock.isAlarm1())
    {
        buf_signal = float(signal);
        if(mkrentgen == 0.0)
        {
            if(iter < 46)
            {
                buf_rentgen[iter] = buf_signal;
                signal = 0;
                sum = 0;
                Serial.print("BufRad: ");Serial.print(buf_rentgen[iter]);Serial.println(" uR");
            }
            iter++;
            if(iter == 46)
            {
                for (int i = 0; i < 46; ++i)
                {
                    mkrentgen = mkrentgen + buf_rentgen[i];
                }
                mkrentgen = mkrentgen/2.0;
                Serial.print("Rad: ");Serial.print(mkrentgen);Serial.println(" uR");
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
            sum = 0;
            Serial.print("Rad: ");Serial.print(mkrentgen);Serial.println(" uR");
            iter++;
            if(iter == 46)
            {
                iter = 0;
            }
        }
        RTCDateTime dt = clock.getDateTime();
        container.time = dt.unixtime;
        container.temperature = getTemperature(BOARD_TEMP);
        container.temperature_bar = getTemperature(BAR_TEMP);
        container = get_data_stm(container);
        int result = bar.read();
        if (result != 0)
        {
            Serial.println("Failed");
            container.pressure = 0.0;
        }
        else
        {
            container.pressure = bar.getPressure();
        }
        result = bar1.read();
        if (result != 0)
        {
            Serial.println("Failed");
        }
        else
        {
            container.pressure = (bar.getPressure() + container.pressure) / 2 ;
        }
        container.rad = mkrentgen;
        Serial.print("Time = ");Serial.println(container.time);
        Serial.print("Temperature = ");Serial.println(container.temperature);
        Serial.print("Temperature1 = ");Serial.println(container.temperature_bar);
        Serial.print("Pressure = ");Serial.println(container.pressure);
        Serial.print("Temperature2 = ");Serial.println(container.temp_stm);
        Serial.print("Pulse Width = ");Serial.println(container.pw);
        Serial.print("Polarity = ");Serial.println(container.polarity);
        log_data(container);
    }
}
