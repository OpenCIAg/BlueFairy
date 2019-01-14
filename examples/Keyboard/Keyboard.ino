#include "Scheduler.h"
#include "Keyboard.h"

const int LED_PIN = 13;
int LED_VALUE = 0;

arc::Scheduler scheduler;
arc::Keyboard<2> keyboard((byte[]){0,1});

enum Button {
    On=0,
    Off
};

void toggleLed(){
    LED_VALUE = (LED_VALUE + 1) % 2;
    digitalWrite(LED_PIN, LED_VALUE);
}

void setup(){
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LED_VALUE);
    scheduler.every(50, [](arc::Scheduler& scheduler){
        keyboard.tick();
    });
    keyboard.onKeyDown[On] = []() {
        Serial.println("onKeyDown(On)");
    };
    keyboard.onKeyUp[On] = []() {
        Serial.println("onKeyUp(On)");
    };
    keyboard.onKeyDown[Off] = []() {
        Serial.println("onKeyDown(Off)");
    };
    keyboard.onKeyUp[Off] = []() {
        Serial.println("onKeyUp(Off)");
    };
}

void loop(){
    scheduler.loop();
}