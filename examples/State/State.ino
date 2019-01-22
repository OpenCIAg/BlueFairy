#include "State.h"
#include "Scheduler.h"
#include "Keyboard.h"
#include "ciag.h"

const int LED_PIN = 13;
int LED_VALUE = 0;

void toggleLed(){
    LED_VALUE = (LED_VALUE + 1) % 2;
    digitalWrite(LED_PIN, LED_VALUE);
}

ciag::Scheduler scheduler;
ciag::Keyboard<2> keyboard((byte[]){0, 1});
ciag::StateMachine<2> stateMachine;
ciag::Scheduler::Group blinkAnnimation =  scheduler.group();

enum Button {
    Next = 0,
    Prev
};

enum AppState {
    INIT=0,
    FIRST
};


class InitState : public ciag::State {
public:
    void enter(){
        blinkAnnimation
            .every(500, toggleLed)
            .every( 250, 500, toggleLed);
        keyboard.onKeyUp[Next] = [](const ciag::KeyEvent& keyEvent) {
            stateMachine.toState(FIRST);
        };
    }

    void leave(){
        keyboard.clear();
        blinkAnnimation.cancel();
    }
};

InitState initState;

void setup() {
    Serial.begin(9600);
    stateMachine[AppState::INIT] = new ciag::DebugStateDecorator<InitState&>("INIT",Serial, initState);
    stateMachine[AppState::FIRST] = new ciag::DebugStateDecorator<ciag::GenericState<>>("FIRST",Serial, ciag::makeState(
        (ciag::runnable)[](){
            blinkAnnimation
                .every(250, toggleLed)
                .every( 125, 250, toggleLed);
            keyboard.onKeyUp[Next] = [](const ciag::KeyEvent& keyEvent) {
                stateMachine.toState(FIRST);
            };
            keyboard.onKeyUp[Prev] = [](const ciag::KeyEvent& keyEvent) {
                stateMachine.toState(INIT);
            };
        },
        (ciag::runnable)[](){
            keyboard.clear();
            blinkAnnimation.cancel();
        }
    ));
    scheduler.every(2000,[](){ keyboard.tick(); });
    stateMachine.toState(AppState::INIT);
}


void loop() {
    scheduler.loop();
}