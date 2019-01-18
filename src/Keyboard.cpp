
#include "Keyboard.h"

namespace arc {

    KeyEvent::KeyEvent(size_t key, Edge edge, unsigned int holdTicks, unsigned long lastChange, bool value) {
        this->key = key;
        this->edge = edge;
        this->holdTicks = holdTicks;
        this->lastChange = lastChange;
        this->value = value;
    }

    unsigned int KeyEvent::holdTime() const {
        return millis() - this->lastChange;
    }

    void NullAction::perform(const KeyEvent&){}

    KeyActionHolder::KeyActionHolder(){
        this->keyAction = new NullAction();
    }

    void KeyActionHolder::operator()(const KeyEvent& keyEvent){
        this->keyAction->perform(keyEvent);
    }



}