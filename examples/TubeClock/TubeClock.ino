#include "TubeDisplay.h"
#include "bluefairy.h"

bluefairy::Scheduler scheduler;

TubeDisplay tubeDisplay;

void setup() {
    tubeDisplay.turnOff();
    scheduler.every(4, [](){ tubeDisplay.tick(); });
}

void loop() {
    scheduler.tick();
}