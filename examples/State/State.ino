#include "State.h"
#include "Scheduler.h"
#include "Keyboard.h"
#include "arc.h"

const int LED_PIN = 13;
int LED_VALUE = 0;

void toggleLed(){
    LED_VALUE = (LED_VALUE + 1) % 2;
    digitalWrite(LED_PIN, LED_VALUE);
}

arc::Scheduler scheduler;
arc::Keyboard<2> keyboard((byte[]){0, 1});
arc::StateMachine<2> stateMachine;
arc::Scheduler::Group blinkAnnimation =  scheduler.group();

enum Button {
    Next = 0,
    Prev
};

enum AppState {
    INIT=0,
    FIRST
};


class InitState : public arc::State {
public:
    void enter(){
        blinkAnnimation
            .every(500, toggleLed)
            .every( 250, 500, toggleLed);
        keyboard.onKeyUp[Next] = [](const arc::KeyEvent& keyEvent) {
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
    stateMachine[AppState::INIT] = new arc::DebugState<InitState&>("INIT",Serial, initState);
    stateMachine[AppState::FIRST] = new arc::DebugState<arc::GenericState<>>("FIRST",Serial, arc::makeState(
        (arc::runnable)[](){
            blinkAnnimation
                .every(250, toggleLed)
                .every( 125, 250, toggleLed);
            keyboard.onKeyUp[Next] = [](const arc::KeyEvent& keyEvent) {
                stateMachine.toState(FIRST);
            };
            keyboard.onKeyUp[Prev] = [](const arc::KeyEvent& keyEvent) {
                stateMachine.toState(INIT);
            };
        },
        (arc::runnable)[](){
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