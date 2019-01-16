#include "State.h"
#include "Scheduler.h"
#include "Keyboard.h"
#include "arc.h"

arc::Scheduler scheduler;
arc::Keyboard<2> keyboard((byte[]){0, 1});

typedef struct InputOutput {
    Stream& serial;
    arc::Keyboard<2>& keyboard;
} IO;

IO io = {
    .serial=Serial,
    .keyboard=keyboard
};

arc::StateMachine<1,IO&> stateMachine(scheduler, io);

enum Button {
    Next = 0,
    Prev
};

enum AppState {
    INIT=0,
    FIRST
};

class InitState : public arc::State<IO&> {
protected:
    arc::TaskNode * keyboardTask = NULL;
public:
    void enter(arc::Scheduler& scheduler, IO& io){
        this->keyboardTask = scheduler.every(50, [io]() {
            io.keyboard.tick();
        });
        io.keyboard.onKeyUp[Next] = [](const arc::KeyEvent& keyEvent) {
            //stateMachine.toState(FIRST);
        };
    }
    void leave(arc::Scheduler& scheduler, IO& io){
        safeClean(this->keyboardTask);
    }
};


arc::TaskNode * blinkTask;


void setup() {
    Serial.begin(9600);
    stateMachine[AppState::INIT] = new InitState();
    stateMachine.toState(AppState::INIT);
}


void loop() {
    scheduler.loop();
}