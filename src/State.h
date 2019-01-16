#ifndef __ARC_STATE__
#define __ARC_STATE__

#include "Scheduler.h"


namespace arc {

    template<typename IO>
    class State {
    public:
        virtual void enter(Scheduler& scheduler, IO io) =0;
        virtual void out(Scheduler& scheduler, IO io) = 0;
        virtual ~State(){};
    };

    template<typename IO>
    class NullState : public State<IO> {
        void enter(Scheduler&,IO){};
        void out(Scheduler&,IO){};
    };

    template<size_t SIZE, typename IO>
    class StateMachine {
    public:
        StateMachine(Scheduler&,IO);
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
    }

    template<size_t SIZE, typename IO>
    void StateMachine<SIZE, IO>::toState(size_t stateIndex) {
        this->currentState->out(this->scheduler, this->io);
        this->currentState = this->states[stateIndex];
        this->currentState->enter(this->scheduler, this->io);
    }

    template<size_t SIZE, typename IO>
    State<IO>*& StateMachine<SIZE,IO>::operator[](size_t stateIndex) {
        return this->states[stateIndex];
    }

}


#endif