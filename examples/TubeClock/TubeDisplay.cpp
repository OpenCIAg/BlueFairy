#include "TubeDisplay.h"

using namespace bluefairy;

TubeDisplay::TubeDisplay(
    unsigned int totalDigits,
    DigitalOutput** numberSelector,
    DigitalOutput** digitSelector
) {
    this->totalDigits = totalDigits;
    this->numberSelector = numberSelector;
    this->digitSelector = digitSelector;
    this->value = new unsigned char[totalDigits];
}

TubeDisplay::~TubeDisplay(){
    this->turnOff();
    delete[] this->value;
}

void TubeDisplay::setValue(const unsigned char* const value) {
    for(int i=0;i<this->totalDigits;i+=1){
        this->value[i] = value[i];
    }
    this->draw();
}

const unsigned char* const TubeDisplay::getValue(){
    return this->value;
}

void TubeDisplay::tick() {
    this->nextState();
    this->draw();    
}

void TubeDisplay::nextState() {
    this->state = (this->state + 1) % this->totalDigits;
}

void TubeDisplay::draw() {
    this->turnOff();
    const char currentNumber = this->value[this->state] % 10;
    this->digitSelector[this->state]->write(true);
    this->numberSelector[currentNumber]->write(true);
}

void TubeDisplay::turnOff() {
    for(int i =0;i<this->totalDigits;i+=1){
        this->digitSelector[i]->write(false);
    }
    for(int i=0;i < 10;i+=1){
        this->numberSelector[i]->write(false);
    }
}