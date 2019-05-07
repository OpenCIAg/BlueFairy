#ifndef __TUBE_DISPLAY__
#define __TUBE_DISPLAY__

#include "IO.h"

class TubeDisplay {
protected:
    unsigned int totalDigits;
    unsigned int state;
    unsigned char value[4];
    DigitalOutput* numberSelector[10];
    DigitalOutput* digitSelector[4];
public:
    TubeDisplay();
    ~TubeDisplay();
    void setValue(const unsigned char* const value);
    const unsigned char* const getValue();
    void tick();
    void draw();
    void nextState();
    void turnOff();
};

#endif