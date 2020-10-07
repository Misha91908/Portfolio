#include "Arduino.h"
#include "Wire.h"

void requestEvent()
{
    Wire.write((uint8_t)map(analogRead(1), 0, 1023, 0, 255)); // (1.1/255)*result*11
    Wire.write((uint16_t)analogRead(3)); //
}

void setup()
{
    analogReference(INTERNAL);
    Wire.begin(0x03);
    Wire.onRequest(requestEvent);
    pinMode(7, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(7, HIGH);
    digitalWrite(6, HIGH);
}

void loop()
{
    delay(10);
}

