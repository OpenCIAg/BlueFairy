#ifndef __TUBE_DISPLAY__
#define __TUBE_DISPLAY__

#include "bluefairy.h"

class TubeDisplay {
protected:
    unsigned int totalDigits;
    unsigned int state;
    unsigned char* value;
    bluefairy::DigitalOutput** numberSelector;
    bluefairy::DigitalOutput** digitSelector;
public:
    TubeDisplay(
        unsigned int totalDigits,
        bluefairy::DigitalOutput** numberSelector,
        bluefairy::DigitalOutput** digitSelector
    );
    ~TubeDisplay();
    void setValue(const unsigned char* const value);
    const unsigned char* const getValue();
    void tick();
    void draw();
    void nextState();
    void turnOff();
};

#endif