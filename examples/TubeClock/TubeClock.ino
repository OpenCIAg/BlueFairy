#include "TubeDisplay.h"
#include "bluefairy.h"

bluefairy::Scheduler scheduler;

auto Digit0 = bluefairy::IO.digitalOutput(A0);
auto Digit1 = bluefairy::IO.digitalOutput(A1);
auto Digit2 = bluefairy::IO.digitalOutput(A2);
auto Digit3 = bluefairy::IO.digitalOutput(A3);

auto Number0 = bluefairy::IO.digitalOutput(2);
auto Number1 = bluefairy::IO.digitalOutput(3);
auto Number2 = bluefairy::IO.digitalOutput(6);
auto Number3 = bluefairy::IO.digitalOutput(7);
auto Number4 = bluefairy::IO.digitalOutput(8);
auto Number5 = bluefairy::IO.digitalOutput(9);
auto Number6 = bluefairy::IO.digitalOutput(10);
auto Number7 = bluefairy::IO.digitalOutput(11);
auto Number8 = bluefairy::IO.digitalOutput(12);
auto Number9 = bluefairy::IO.digitalOutput(13);

bluefairy::DigitalOutput* digits[4] = {
    &Digit0,
    &Digit1,
    &Digit2,
    &Digit3
};

bluefairy::DigitalOutput* numbers[10] = {
    &Number0,
    &Number1,
    &Number2,
    &Number3,
    &Number4,
    &Number5,
    &Number6,
    &Number7,
    &Number8,
    &Number9
};

TubeDisplay tubeDisplay(
    4, numbers, digits
);

void setup() {
    scheduler.every(4, [](){ tubeDisplay.tick(); });
    scheduler.every(250, [](){
        unsigned long elapsedTime = millis()/1000.0;
        unsigned char newValue[4] = { 0, 0, 0 , 0 };
        for( int i =3;i>=0;i-= 1){
            const unsigned char number = ((unsigned long)(elapsedTime/pow(10,i))) % 10;
            Serial.print(number,DEC);
            newValue[i] = number;
        }
        Serial.println();
        tubeDisplay.setValue(newValue);
    });
}   

void loop() {
    scheduler.tick();
}