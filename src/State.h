#ifndef __CIAG__BLUEFAIRY__STATE__
#define __CIAG__BLUEFAIRY__STATE__

#include "Arduino.h"
#include "Scheduler.h"


namespace ciag {
    namespace bluefairy {

        typedef void (*runnable)();

        class State {
        public:
            virtual void enter() =0;
            virtual void leave() = 0;
            virtual ~State(){};
        };

        template<typename S>
        class DebugStateDecorator : public State{
        public:
            DebugStateDecorator(const char * const name, Stream& output, S state ) : name(name), output(output), state(state) {

            }

            DebugStateDecorator(const DebugStateDecorator<S>& orig) : name(orig.name), output(orig.output), state(orig.state) {

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
        };

        template<typename EF =runnable, typename LF=runnable>
        class FunctionalState {
        public:
            FunctionalState(EF enterFunction, LF leaveFunction) : enterFunction(enterFunction), leaveFunction(leaveFunction) {}
            void enter() { this->enterFunction(); }
            void leave() { this->leaveFunction(); }
        protected:
            EF enterFunction;
            LF leaveFunction;
        };

        template<typename EF=runnable, typename LF=runnable>
        FunctionalState<EF,LF> makeState(EF enterFunction, LF leaveFunction) {
            return FunctionalState<EF,LF>(enterFunction, leaveFunction);
        }

        template<typename EF=runnable, typename LF=runnable>
        DebugStateDecorator< FunctionalState<EF,LF> > makeState(Stream& output,const char * const name, EF enterFunction, LF leaveFunction){
            return DebugStateDecorator< FunctionalState<EF,LF> >(name, output, makeState(enterFunction, leaveFunction));
        }

        template<typename E>
        DebugStateDecorator<E> makeState(Stream& output, const char * const name, E state){
            return DebugStateDecorator< E >(name, output, state);
        }

        class NullState : public State {
            void enter(){};
            void leave(){};
        };

        template<typename E>
        class TemplateState: public State {
        public:
            TemplateState(E state) : state(state){} ;
            void enter(){ this->state.enter(); }
            void leave(){ this->state.leave(); }
        protected:
            E state;
        };

        class StateHolder : public State{
        public:
            StateHolder();
            template<typename E>
            StateHolder& operator=(E innerState);
            void enter();
            void leave();
            void clear();
        protected:
            State * state;
        };

        template<typename E>
        StateHolder& StateHolder::operator=(E innerState){
            this->clear();
            this->state = new TemplateState<E>(innerState);
            return *this;
        }

        template<size_t SIZE>
        class StateMachine {
        public:
            StateMachine();
            ~StateMachine();
            void toState(size_t stateIndex);
            StateHolder& operator[](size_t stateIndex);
        protected:
            size_t currentState;
            StateHolder states[SIZE];
        };

        template<size_t SIZE>
        StateMachine<SIZE>::StateMachine(){
            this->currentState = 0;
        }

        template<size_t SIZE>
        StateMachine<SIZE>::~StateMachine(){
            for(size_t i = 0; i > SIZE ; ++i) {
                this->states[i].clear();
            }
        }

        template<size_t SIZE>
        void StateMachine<SIZE>::toState(size_t stateIndex) {
            this->states[currentState].leave();
            this->currentState = stateIndex;
            this->states[currentState].enter();
        }

        template<size_t SIZE>
        StateHolder& StateMachine<SIZE>::operator[](size_t stateIndex) {
            return this->states[stateIndex];
        }

    }
}

#endif