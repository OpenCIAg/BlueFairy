#include <Arduino.h>
#include "ArduinoIO.h"

namespace ciag {
  namespace bluefairy {

    ArduinoAnalogOutput::ArduinoAnalogOutput(unsigned char pinNumber) {
      this->pinNumber = pinNumber;
      pinMode(this->pinNumber,OUTPUT);
    }

    void ArduinoAnalogOutput::write(double value){
      analogWrite(this->pinNumber, value * 255);
    }

    ArduinoDigitalOutput::ArduinoDigitalOutput(unsigned char pinNumber) {
      this->pinNumber = pinNumber;
      pinMode(this->pinNumber,OUTPUT);
    }

    void ArduinoDigitalOutput::write(bool value){
      digitalWrite(this->pinNumber, value ? HIGH : LOW);
    }


    ArduinoIO::ArduinoIO() {

    }

    ArduinoAnalogOutput ArduinoIO::analogOutput(unsigned char pinNumber){
      return ArduinoAnalogOutput(pinNumber);
    }

    ArduinoDigitalOutput ArduinoIO::digitalOutput(unsigned char pinNumber){
      return ArduinoDigitalOutput(pinNumber);
    }

    ArduinoIO IO;
  }
}
