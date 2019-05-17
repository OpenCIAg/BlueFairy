#include "TubeDisplay.h"

TubeDisplay::TubeDisplay(
    unsigned int totalDigits,
    bluefairy::DigitalOutput** numberSelector,
    bluefairy::DigitalOutput** digitSelector
) {
    this->totalDigits = totalDigits;
    this->numberSelector = numberSelector;
    this->digitSelector = digitSelector;
    this->value = new unsigned char[totalDigits];
    this->active = new bool[totalDigits]{true,true,true,true};
    this->turnOff();
}

TubeDisplay::~TubeDisplay(){
    this->turnOff();
    delete[] this->value;
    delete[] this->active;
}

void TubeDisplay::setValue(const unsigned char* const value) {
    for(int i=0;i<this->totalDigits;i+=1){
        this->value[i] = value[i];
    }
    this->draw();
}

const unsigned char* const TubeDisplay::getValue() const{
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
    const bool active = this->active[this->state];
    this->digitSelector[this->state]->write(active);
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

void TubeDisplay::toggleDigit(unsigned char index) {
    const unsigned char safeIndex = index % this->totalDigits;
    this->active[safeIndex] = !this->active[safeIndex];
}

void TubeDisplay::setDigitActive(unsigned char index, bool value){
    const unsigned char safeIndex = index % this->totalDigits;
    this->active[safeIndex] = value;
}

bool TubeDisplay::isDigitActive(unsigned char index) const {
    const unsigned char safeIndex = index % this->totalDigits;
    return this->active[safeIndex];
}