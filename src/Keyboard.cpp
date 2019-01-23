
#include "Keyboard.h"

namespace ciag {
    namespace bluefairy {

        NullAction NULL_ACTION;

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
            this->keyAction = &NULL_ACTION;
        }

        KeyActionHolder::~KeyActionHolder(){
            this->clear();
        }

        void KeyActionHolder::operator()(const KeyEvent& keyEvent){
            this->keyAction->perform(keyEvent);
        }

        void KeyActionHolder::clear(){
            if(this->keyAction == &NULL_ACTION){
                return;
            }
            delete this->keyAction;
            this->keyAction = &NULL_ACTION;
        }

    }
}