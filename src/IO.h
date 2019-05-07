#ifndef __IO_H__
#define __IO_H__

class AnalogOutput {
public:
    virtual void write(double value) = 0;
};

class DigitalOutput{
public:
    virtual void write(bool value) = 0;
};

#ifdef __AVR__
#include "io/ArduinoIO.h"
#endif
#ifdef ESP32
#include "io/ESP32IO.h"
#endif

#endif