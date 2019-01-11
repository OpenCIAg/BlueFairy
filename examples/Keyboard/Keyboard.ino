#include "Scheduler.h"
#include "EdgeDetector.h"

const int LED_PIN = 13;
int LED_VALUE = 0;

arc::Scheduler scheduler;
arc::EdgeDetector<1> keyboard({0});

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
    keyboard.trigger = [](const arc::Edge* const edges) {
        if(edges[0] == arc::Edge::Negedge) {
            toggleLed();
        }
    };
}

void loop(){
    scheduler.loop();
}