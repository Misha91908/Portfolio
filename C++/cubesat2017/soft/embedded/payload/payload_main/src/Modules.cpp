#include <Arduino.h>
#include <SPI.h>
#include "Modules.h"
#include "SD.h"
#include "Wire.h"

float getTemperature(int pin)
{
    float rawValue = analogRead(pin);

    float voltage = rawValue / 1024.0 * 2.56;
    float temperatureC = (voltage - 0.5) * 100;

    return temperatureC;
}

PayloadData get_data_stm(PayloadData data)
{
    char p[12];
    binaryFloat dataFloat;
    binaryLong dataLong;
    int counter = 0;
    while (!Serial1.available())
    {
        counter++;
        if (counter == 1000)
        {
            data.temp_stm = 0.0;
            data.pw = 0.0;
            data.polarity = 0;
            return data;
        }
    }
    for (int i = 0; i < 12; ++i)
    {
        p[i] = Serial1.read();
    }
    dataFloat.bin[0] = p[0];
    dataFloat.bin[1] = p[1];
    dataFloat.bin[2] = p[2];
    dataFloat.bin[3] = p[3];
    data.temp_stm = dataFloat.value;
    dataFloat.bin[0] = p[4];
    dataFloat.bin[1] = p[5];
    dataFloat.bin[2] = p[6];
    dataFloat.bin[3] = p[7];
    data.pw = dataFloat.value;
    dataLong.bin[0] = p[8];
    dataLong.bin[1] = p[9];
    dataLong.bin[2] = p[10];
    dataLong.bin[3] = p[11];
    data.polarity = dataLong.value;
    return data;
}

void log_data(PayloadData data)
{
    File f = SD.open("LOG.txt", FILE_WRITE);
    if(f)
    {
        f.print(data.time);f.print(";");
        f.print(data.temperature);f.print(";");
        f.print(data.temperature_bar);f.print(";");
        f.print(data.pressure);f.print(";");
        f.print(data.temp_stm);f.print(";");
        f.print(data.pw);f.print(";");
        f.print(data.polarity);f.print(";");
        f.println(data.rad);
        f.close();
    }
    Serial.println("Successful logging data!");
}
