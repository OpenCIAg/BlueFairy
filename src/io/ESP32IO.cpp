#include "ESP32IO.h"

#ifdef ESP32

ESP32IO::ESP32IO(){

}

ESP32AnalogOutout ESP32IO::analogOutput(unsigned char pinNumber){
  return ESP32AnalogOutout(pinNumber);
}

unsigned int ESP32AnalogOutout::calcDutyCycle(double value){
    unsigned int maxValue = pow(2,this->bitResolution) -1;
    double preparedValue = max(min(1., value), 0.);
    return round(preparedValue * maxValue);
}

ESP32AnalogOutout::ESP32AnalogOutout(unsigned char pinNumber){
  this->channel = 0;
  this->bitResolution = 8;
  const unsigned int frequency = 5000;
  this->pinNumber = pinNumber;
  ledcSetup(this->channel,frequency,this->bitResolution);
  ledcAttachPin(this->pinNumber, channel);
}

void ESP32AnalogOutout::write(double value) {
  unsigned int dutyCycle = this->calcDutyCycle(value);
  ledcWrite(this->channel, dutyCycle);
}


ESP32IO IO;

#endif