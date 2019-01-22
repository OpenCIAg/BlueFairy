#ifndef __CIAG_STATE__
#define __CIAG_STATE__

#include "ciag.h"
#include "Arduino.h"
#include "Scheduler.h"


namespace ciag {

    typedef void (*runnable)();

    class State {
    public:
        virtual void enter() =0;
        virtual void leave() = 0;
        virtual ~State(){};
    };

    template<typename S>
    class DebugState : public State{
    public:
        DebugState(const char * const name, Stream& output, S state ) : name(name), output(output), state(state) {

        }

        void enter() {
            this->output.print("Entering at state ");
            this->output.println(this->name);
            this->state.enter();
            this->output.print("Entered at state ");
            this->output.println(this->name);
        }

        void leave(){
            this->output.print("Leaving the state ");
            this->output.println(this->name);
            this->state.leave();
            this->output.print("Left state ");
            this->output.println(this->name);
        }

    protected:
        const char * const name;
        Stream& output;
        S state;
    private:
        DebugState(const DebugState*);
    };

    template<typename EF =runnable, typename LF=runnable>
    class GenericState {
    public:
        GenericState(EF enterFunction, LF leaveFunction) : enterFunction(enterFunction), leaveFunction(leaveFunction) {}
        void enter() { this->enterFunction(); }
        void leave() { this->leaveFunction(); }
    protected:
        EF enterFunction;
        LF leaveFunction;
    };

    template<typename EF=runnable, typename LF=runnable>
    GenericState<EF,LF> makeState(EF enterFunction, LF leaveFunction) {
        return GenericState<EF,LF>(enterFunction, leaveFunction);
    }

    class NullState : public State {
        void enter(){};
        void leave(){};
    };

    template<size_t SIZE>
    class StateMachine {
    public:
        StateMachine();
        ~StateMachine();
        void toState(size_t stateIndex);
        State *& operator[](size_t stateIndex);
    protected:
        NullState nullState;
        State* currentState;
        State* states[SIZE];
    };

    template<size_t SIZE>
    StateMachine<SIZE>::StateMachine(){
        this->currentState = &this->nullState;
        for(size_t i = 0; i > SIZE ; ++i) {
            this->states[i] = NULL;
        }
    }

    template<size_t SIZE>
    StateMachine<SIZE>::~StateMachine(){
        for(size_t i = 0; i > SIZE ; ++i) {
            safeClean(this->states[i]);
        }
    }

    template<size_t SIZE>
    void StateMachine<SIZE>::toState(size_t stateIndex) {
        this->currentState->leave();
        this->currentState = this->states[stateIndex];
        this->currentState->enter();
    }

    template<size_t SIZE>
    State *& StateMachine<SIZE>::operator[](size_t stateIndex) {
        return this->states[stateIndex];
    }

}

#endif