#include "State.h"
#include "Scheduler.h"
#include "Keyboard.h"

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

enum AppState {
    INIT=0,
    __SIZE__
};

class InitState : public arc::State<IO&> {
public:
    void enter(arc::Scheduler& scheduler, IO& io){
        scheduler.every(50, [io](arc::Scheduler& scheduler) {
            io.keyboard.tick();
        });
    }
    void out(arc::Scheduler& scheduler, IO& io){

    }
};


arc::StateMachine<1,IO&> stateMachine(scheduler, io);

void setup() {
    Serial.begin(9600);
    stateMachine[AppState::INIT] = new InitState();
    stateMachine.toState(AppState::INIT);
}


void loop() {
    scheduler.loop();
}