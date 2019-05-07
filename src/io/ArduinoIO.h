#ifndef __IO__ARDUINOIO__
#define __IO__ARDUINOIO__


#include <Arduino.h>
#include "../IO.h"

namespace ciag {
    namespace bluefairy {

        class ArduinoAnalogOutput : public AnalogOutput {
        protected:
            unsigned char pinNumber;
        public:
            ArduinoAnalogOutput(unsigned char pinNumber);
            void write(double value);
        };

        class ArduinoDigitalOutput : public DigitalOutput {
        protected:
            unsigned char pinNumber;
        public:
            ArduinoDigitalOutput(unsigned char pinNumber);
            void write(bool value);
        };

        class ArduinoIO {
        public:
            ArduinoIO();
            ArduinoAnalogOutput analogOutput(unsigned char pinNumber);
            ArduinoDigitalOutput digitalOutput(unsigned char pinNumber);
        };

        extern ArduinoIO IO;
    }
}

#endif