#include "State.h"

namespace ciag {
    namespace bluefairy {

        NullState NULL_STATE;

        StateHolder::StateHolder() {
            this->state = &NULL_STATE;
        }

        void StateHolder::enter() {
            this->state->enter();
        }

        void StateHolder::leave() {
            this->state->leave();
        }

        void StateHolder::clear(){
            if(this->state == &NULL_STATE){
                return;
            }
            delete this->state;
            this->state = &NULL_STATE;
        }

    }
}