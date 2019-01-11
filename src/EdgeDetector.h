#ifndef __ARC__EDGE_DETECTOR__
#define __ARC__EDGE_DETECTOR__

#include "Arduino.h"

namespace arc {

    typedef unsigned char byte;

    enum Edge {
        None = 0,
        Negedge = -1,
        Posedge = 1
    };
    
    typedef void (*trigger_t)(const Edge* const);

    template<size_t SIZE>
    class EdgeDetector {
    public:
        EdgeDetector(const byte (&pins)[SIZE]);
        trigger_t trigger = [](const Edge* const edges){};
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

    template<size_t SIZE>
    EdgeDetector<SIZE>::EdgeDetector(const byte (&pins)[SIZE]) {
        this->pins = pins;
    }


    template<size_t SIZE>
    bool EdgeDetector<SIZE>::operator[](size_t index) const{
        return this->pinsValues[index];
    }

    template<size_t SIZE>
    void EdgeDetector<SIZE>::tick() {
        this->readAndCalcAll();
    }

    template<size_t SIZE>
    void EdgeDetector<SIZE>::loop() {
        this->loop(50);
    }

    template<size_t SIZE>
    void EdgeDetector<SIZE>::loop(unsigned int intervalMs) {
        for(;;){
            this->tick();
        }
    }


    template<size_t SIZE>
    void EdgeDetector<SIZE>::init() {
        for(size_t i = 0; i < SIZE ; ++i){
            pinMode(this->pins[i], INPUT);
            this->pinsValues[i] = digitalRead(i);
        }
    }

    template<size_t SIZE>
    bool EdgeDetector<SIZE>::justRead(const size_t pin) {
        return digitalRead(this->pins[pin]);
    }

    template<size_t SIZE>
    Edge EdgeDetector<SIZE>::readAndCalc(const size_t pin) {
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

    template<size_t SIZE>
    void EdgeDetector<SIZE>::readAndCalcAll() {
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


#endif