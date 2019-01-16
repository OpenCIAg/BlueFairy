#include "State.h"
#include "Scheduler.h"
#include "Keyboard.h"
#include "arc.h"

arc::Scheduler scheduler;
arc::Keyboard<2> keyboard((byte[]){0, 1});
arc::StateMachine<2> stateMachine;

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
        keyboard.onKeyUp[Next] = [](const arc::KeyEvent& keyEvent) {
            stateMachine.toState(FIRST);
        };
    }
    void leave(){
        keyboard.clear();
    }
};

void setup() {
    Serial.begin(9600);
    stateMachine[AppState::INIT] = new arc::DebugState<InitState>("INIT",Serial,InitState());
    stateMachine[AppState::FIRST] = new arc::DebugState<arc::GenericState<>>("FIRST",Serial, arc::makeState(
        (arc::runnable)[](){
            keyboard.onKeyUp[Next] = [](const arc::KeyEvent& keyEvent) {
                stateMachine.toState(FIRST);
            };
            keyboard.onKeyUp[Prev] = [](const arc::KeyEvent& keyEvent) {
                stateMachine.toState(INIT);
            };
        },
        (arc::runnable)[](){
            keyboard.clear();
        }
    ));
    stateMachine.toState(AppState::INIT);
    scheduler.every(50,keyboard);
}


void loop() {
    scheduler.loop();
}