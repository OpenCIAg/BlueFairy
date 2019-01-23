#include <bluefairy.h>

const int LED_PIN = 13;
int LED_VALUE = 0;

bluefairy::Scheduler scheduler;
bluefairy::Keyboard<2> keyboard((byte[]){0,1});

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
    scheduler.every(50, [](){
        keyboard.tick();
    });
    keyboard.onKeyDown[On] = [](const bluefairy::KeyEvent& event) {
        Serial.println("onKeyDown(On)");
        Serial.print("HoldTicks: ");
        Serial.println(event.holdTicks);
        Serial.print("LastChange: ");
        Serial.println(event.lastChange);
    };
    keyboard.onKeyUp[On] = [](const bluefairy::KeyEvent& event) {
        Serial.println("onKeyUp(On)");
        Serial.print("HoldTicks: ");
        Serial.println(event.holdTicks);
        Serial.print("LastChange: ");
        Serial.println(event.lastChange);
    };
    keyboard.onHoldDown[On] = [](const bluefairy::KeyEvent& event) {
        Serial.println("onHoldDown(On)");
        Serial.print("HoldTicks: ");
        Serial.println(event.holdTicks);
        Serial.print("LastChange: ");
        Serial.println(event.lastChange);
    };
    keyboard.onKeyDown[Off] = [](const bluefairy::KeyEvent& event) {
        Serial.println("onKeyDown(Off)");
        Serial.print("HoldTicks: ");
        Serial.println(event.holdTicks);
        Serial.print("LastChange: ");
        Serial.println(event.lastChange);
    };
    keyboard.onKeyUp[Off] = [](const bluefairy::KeyEvent& event) {
        Serial.println("onKeyUp(Off)");
        Serial.print("HoldTicks: ");
        Serial.println(event.holdTicks);
        Serial.print("LastChange: ");
        Serial.println(event.lastChange);
    };
    keyboard.onHoldDown[Off] = [](const bluefairy::KeyEvent& event) {
        Serial.println("onHoldDown(Off)");
        Serial.print("HoldTicks: ");
        Serial.println(event.holdTicks);
        Serial.print("LastChange: ");
        Serial.println(event.lastChange);
    };
}

void loop(){
    scheduler.loop();
}