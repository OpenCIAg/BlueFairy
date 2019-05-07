#ifndef __IO_H__
#define __IO_H__

namespace ciag {

    namespace bluefairy {

        class AnalogOutput {
        public:
            virtual void write(double value) = 0;
        };

        class DigitalOutput{
        public:
            virtual void write(bool value) = 0;
        };

    }

}

#ifdef ESP32
#include "io/ESP32IO.h"
#else
#include "io/ArduinoIO.h"
#endif

#endif