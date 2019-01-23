#include <bluefairy.h>

const int LED_PIN = 13;
int LED_VALUE = 0;

void toggleLed(){
    LED_VALUE = (LED_VALUE + 1) % 2;
    digitalWrite(LED_PIN, LED_VALUE);
}

bluefairy::Scheduler scheduler;
bluefairy::Keyboard<2> keyboard((byte[]){0, 1});
bluefairy::StateMachine<2> stateMachine;
bluefairy::Scheduler::Group blinkAnnimation =  scheduler.group();

enum Button {
    Next = 0,
    Prev
};

enum AppState {
    INIT=0,
    FIRST
};


class InitState : public bluefairy::State {
public:
    void enter(){
        blinkAnnimation
            .every(500, toggleLed)
            .every( 250, 500, toggleLed);
        keyboard.onKeyUp[Next] = [](const bluefairy::KeyEvent& keyEvent) {
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
    stateMachine[AppState::INIT] = bluefairy::makeState<InitState&>(Serial, "INIT", initState);
    stateMachine[AppState::FIRST] = bluefairy::makeState(Serial, "FIRST", 
        [](){
            blinkAnnimation
                .every(250, toggleLed)
                .every( 125, 250, toggleLed);
            keyboard.onKeyUp[Next] = [](const bluefairy::KeyEvent& keyEvent) {
                stateMachine.toState(FIRST);
            };
            keyboard.onKeyUp[Prev] = [](const bluefairy::KeyEvent& keyEvent) {
                stateMachine.toState(INIT);
            };
        },
        [](){
            keyboard.clear();
            blinkAnnimation.cancel();
        }
    );
    scheduler.every(2000,[](){ keyboard.tick(); });
    stateMachine.toState(AppState::INIT);
}


void loop() {
    scheduler.loop();
}