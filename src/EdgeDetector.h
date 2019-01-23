#ifndef __CIAG__BLUEFAIRY__EDGE_DETECTOR__
#define __CIAG__BLUEFAIRY__EDGE_DETECTOR__

#include "Arduino.h"

namespace ciag {
    namespace bluefairy {

        typedef unsigned char byte;

        enum Edge {
            None = 0,
            Negedge = -1,
            Posedge = 1
        };
        
        typedef void (*trigger_t)(const Edge* const);

        template<size_t SIZE, typename C = trigger_t>
        class EdgeDetector {
        public:
            EdgeDetector(const byte* const pins);
            EdgeDetector(const byte* const pins, C trigger);
            C trigger;
            bool operator[] (size_t index) const;
            void tick();
            void loop();
            void loop(unsigned int intervalMs);
        protected:
            bool pinsValues[SIZE];
            byte const * pins;
            void init();
            bool justRead(const size_t pin);
            Edge readAndCalc(const size_t pin);
            void readAndCalcAll();
        };

        template<size_t SIZE, typename C>
        EdgeDetector<SIZE,C>::EdgeDetector(const byte* const pins){
            this->trigger = [](const Edge* const edges){};
            this->pins = pins;
            this->init();
        }

        template<size_t SIZE, typename C>
        EdgeDetector<SIZE,C>::EdgeDetector(const byte* const pins, C trigger) : trigger(trigger) {
            this->pins = pins;
            this->init();
        }


        template<size_t SIZE, typename C>
        bool EdgeDetector<SIZE,C>::operator[](size_t index) const{
            return this->pinsValues[index];
        }

        template<size_t SIZE, typename C>
        void EdgeDetector<SIZE,C>::tick() {
            this->readAndCalcAll();
        }

        template<size_t SIZE, typename C>
        void EdgeDetector<SIZE,C>::loop() {
            this->loop(50);
        }

        template<size_t SIZE, typename C>
        void EdgeDetector<SIZE,C>::loop(unsigned int intervalMs) {
            for(;;){
                this->tick();
            }
        }


        template<size_t SIZE, typename C>
        void EdgeDetector<SIZE,C>::init() {
            for(size_t i = 0; i < SIZE ; ++i){
                pinMode(this->pins[i], INPUT);
                this->pinsValues[i] = digitalRead(this->pins[i]);
            }
        }

        template<size_t SIZE, typename C>
        bool EdgeDetector<SIZE,C>::justRead(const size_t pin) {
            return digitalRead(this->pins[pin]);
        }

        template<size_t SIZE, typename C>
        Edge EdgeDetector<SIZE,C>::readAndCalc(const size_t pin) {
            const auto oldValue = this->pinsValues[pin];
            const auto newValue = this->justRead(pin);
            this->pinsValues[pin] = newValue;
            if( oldValue == newValue) {
                return Edge::None;
            }
            if( newValue) {
                return Edge::Posedge;
            }
            else{
                return Edge::Negedge;
            }
        }

        template<size_t SIZE, typename C>
        void EdgeDetector<SIZE,C>::readAndCalcAll() {
            bool fireTrigger = false;
            Edge edges[SIZE];
            for(size_t i = 0; i < SIZE; ++i) {
                Edge edge = this->readAndCalc(i);
                edges[i] = edge;
                fireTrigger |= (edge != Edge::None);
            }
            if(fireTrigger) {
                this->trigger(edges);
            }
        }

    }
}

#endif