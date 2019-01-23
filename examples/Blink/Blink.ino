#include <bluefairy.h>

const int LED_PIN = 13;
int LED_VALUE = 0;

bluefairy::Scheduler scheduler;

void toggleLed(){
    LED_VALUE = (LED_VALUE + 1) % 2;
    digitalWrite(LED_PIN, LED_VALUE);
}

void setup(){
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LED_VALUE);
    scheduler.repeat(5, 250, 500, toggleLed);
    scheduler.repeat(5, 0, 500, toggleLed);
    scheduler.every(3000, 1000, toggleLed);
}

void loop(){
    scheduler.loop();
}