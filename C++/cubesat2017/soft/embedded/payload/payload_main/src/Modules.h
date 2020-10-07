#ifndef PAYLOAD_TOOLS_H
#define PAYLOAD_TOOLS_H

#define RE 6
#define DE 7

typedef struct
{
    unsigned long time = 0;
    float temperature_bar = 0.0;
    float temperature = 0.0;
    float pressure = 0.0;
    float rad = 0.0;
    float pw = 0.0;
    int polarity = 0;
    float temp_stm = 0.0;
} PayloadData;

typedef union binaryFloat
{
    float value;
    char bin[4];
} binaryFloat;

typedef union binaryULong
{
    unsigned long value;
    char bin[4];
} binaryULong;

typedef union binaryLong
{
    long value;
    char bin[4];
} binaryLong;

void log_data(PayloadData);
float getTemperature(int);
PayloadData get_data_stm(PayloadData);

#endif //PAYLOAD_TOOLS_H