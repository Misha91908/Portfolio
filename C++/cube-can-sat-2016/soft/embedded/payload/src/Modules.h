#ifndef PAYLOAD_TOOLS_H
#define PAYLOAD_TOOLS_H

#define RE 6
#define DE 7

int changeMode(int);
void log_data(int, unsigned long);
void log_rad(float);

typedef union binaryFloat
{
    float value;
    byte bin[4];
}
        binaryFloat;

#endif //PAYLOAD_TOOLS_H