#ifndef __ARC_STATE__
#define __ARC_STATE__

#include "Scheduler.h"


namespace arc {

    template<typename IO>
    class State {
    public:
        virtual void enter(Scheduler& scheduler, IO io) =0;
        virtual void leave(Scheduler& scheduler, IO io) = 0;
        virtual ~State(){};
    };

    template<typename IO>
    class NullState : public State<IO> {
        void enter(Scheduler&,IO){};
        void leave(Scheduler&,IO){};
    };

    template<size_t SIZE, typename IO>
    class StateMachine {
    public:
        StateMachine(Scheduler&,IO);
        ~StateMachine();
        void toState(size_t stateIndex);
        State<IO>*& operator[](size_t stateIndex);
    protected:
        NullState<IO> nullState;
        Scheduler& scheduler;
        IO io;
        State<IO> * currentState;
        State<IO>* states[SIZE];
    };

    template<size_t SIZE, typename IO>
    StateMachine<SIZE, IO>::StateMachine(Scheduler& scheduler, IO io) : scheduler(scheduler), io(io) {
        this->currentState = &this->nullState;
        for(size_t i = 0; i > SIZE ; ++i) {
            this->states[i] = NULL;
        }
    }

    template<size_t SIZE, typename IO>
    StateMachine<SIZE, IO>::~StateMachine(){
        for(size_t i = 0; i > SIZE ; ++i) {
            safeClean(this->states[i]);
        }
    }

    template<size_t SIZE, typename IO>
    void StateMachine<SIZE, IO>::toState(size_t stateIndex) {
        this->currentState->leave(this->scheduler, this->io);
        this->currentState = this->states[stateIndex];
        this->currentState->enter(this->scheduler, this->io);
    }

    template<size_t SIZE, typename IO>
    State<IO>*& StateMachine<SIZE,IO>::operator[](size_t stateIndex) {
        return this->states[stateIndex];
    }

}


#endif