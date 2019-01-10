#include "Scheduler.h"

const int LED_PIN = 13;
int LED_VALUE = 0;

arc::Scheduler scheduler;

void toggleLed(arc::Scheduler &scheduler){
    LED_VALUE = (LED_VALUE + 1) % 2;
    digitalWrite(LED_PIN, LED_VALUE);
}

void setup(){
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LED_VALUE);
    scheduler.every(1000, toggleLed);
}

void loop(){
    scheduler.loop();
}