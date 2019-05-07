#ifndef __IO_ESP32IO__
#define __IO_ESP32IO__

#ifdef ESP32

#include <Arduino.h>
#include "../IO.h"

class ESP32AnalogOutout : public AnalogOutput{
protected:
    unsigned char channel;
    unsigned char bitResolution;
    unsigned char pinNumber;
    unsigned int calcDutyCycle(double value);
public:
    ESP32AnalogOutout(unsigned char pinNumber);
    void write(double value);
};

class ESP32IO{
public:
    ESP32IO();
    ESP32AnalogOutout analogOutput(unsigned char pinNumber);
};

extern ESP32IO IO;

#endif
#endif