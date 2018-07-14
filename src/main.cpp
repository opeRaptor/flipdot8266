#include "Arduino.h"
#include "flipdot8266.h"

Flipdot flipdot(1,20,14); //address Xsize Ysize

int data[20] = {0, 0, 0, 0, 48, 56, 440, 492, 6140, 8184, 7936, 768, 768, 1792, 768, 256, 0, 0, 0, 0}; //dino

void setup() {
        Serial.begin(4800);
}

void loop() {
    flipdot.send();
    delay(1000);
    Serial.println();  //added to see packets in terminal as single line
    delay(1000);
}