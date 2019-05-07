#ifndef __TUBE_DISPLAY__
#define __TUBE_DISPLAY__

#include "bluefairy.h"

class TubeDisplay {
protected:
    unsigned int totalDigits;
    unsigned int state;
    bool* active;
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
    const unsigned char* const getValue() const;
    void tick();
    void draw();
    void nextState();
    void turnOff();
    void toggleDigit(unsigned char index);
    void setDigitActive(unsigned char index, bool value);
    bool isDigitActive(unsigned char index) const;
};

#endif