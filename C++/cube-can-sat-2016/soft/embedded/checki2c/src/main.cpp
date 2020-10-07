#include <Arduino.h>
#include <Wire.h>

void setup() {
   // Wire.begin(); // join i2c bus (address optional for master)
   // Serial.begin(9600); // start serial for output
   // Wire.setClock(40000);
}

void loop() {/*
    Wire.requestFrom(0x03, 3); // request 6 bytes from slave device #8
    if (Wire.available() > 0) {
        while (Wire.available()) { // slave may send less than requested
            uint8_t b = Wire.read();
            //delay(1);// receive a byte as character
            Serial.print(b, HEX); // print the character
            Serial.print(" ");
        }
        Serial.println("");
    }
    else {
        Serial.println("No device.");
    }
    */
    delay(500);
}