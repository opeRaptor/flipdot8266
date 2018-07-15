#include "Arduino.h"
#include "flipdot8266.h"

Flipdot *flipdot = nullptr; //address Xsize Ysize

//Flipdot flipdot1(1,2,3);
//Flipdot flipdot2(1,2,3);

std::vector<int> data = {0, 0, 0, 0, 48, 56, 440, 492, 6140, 8184, 7936, 768, 768, 768, 768, 256, 0, 0, 0, 0}; //dino

void setup() {
        Serial.begin(4800);
        flipdot = new Flipdot(1,20,14); //address Xsize Ysize
}

void loop() {
    flipdot->send(data);
    delay(5000);
    Serial.println();  //added to see packets in terminal as single line
    delay(5000);
}