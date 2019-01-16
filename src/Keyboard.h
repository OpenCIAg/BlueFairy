#ifndef __ARC__KEYBOARD__
#define __ARC__KEYBOARD__
#include "EdgeDetector.h"

namespace arc {

    class KeyEvent {
    public:
        size_t key;
        Edge edge;
        unsigned int holdTicks;
        unsigned long lastChange;
        bool value;

        KeyEvent(size_t key, Edge edge, unsigned int holdTicks, unsigned long lastChange, bool value) {
            this->key = key;
            this->edge = edge;
            this->holdTicks = holdTicks;
            this->lastChange = lastChange;
            this->value = value;
        }

        unsigned int holdTime() const {
            return millis() - this->lastChange;
        }

    };

    typedef void (*key_action_t)(const KeyEvent&);
    key_action_t noop = [](const KeyEvent& event){};


    template<size_t SIZE>
    class Keyboard {
    public:
        Keyboard(const byte* const pins);
        void tick();
        unsigned long lastChange[SIZE];
        unsigned int holdTicks[SIZE];
        key_action_t onKeyDown[SIZE];
        key_action_t onKeyUp[SIZE];
        key_action_t onHoldDown[SIZE];
        void edgeTrigger(const Edge* const edges);
        void operator()(const Edge* const edges);
        void operator()(){ this->tick(); }
        void clear();
        bool isActive() const { return this->active; }
        void turnOff();
        void turnOn();
    protected:
        bool active = true;
        EdgeDetector<SIZE, Keyboard&> edgeDetector;
    };

    template<size_t SIZE>
    Keyboard<SIZE>::Keyboard(const byte* const pins): edgeDetector(pins,*this){
        this->clear();
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::clear() {
        for(size_t i =0;i<SIZE;++i){
            this->onKeyDown[i] = noop;
            this->onKeyUp[i] = noop;
            this->onHoldDown[i] = noop;
            this->holdTicks[i] = 0;
            this->lastChange[i] = 0;
        }
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::turnOff() {
        this->active = false;
        this->clear();
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::turnOn() {
        this->active = true;
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::operator()(const Edge* const edges){
        this->edgeTrigger(edges);
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::edgeTrigger(const Edge* const edges){
        const unsigned long now = millis();
        for(size_t i =0;i<SIZE;i+=1){
            const unsigned int holdTicks = this->holdTicks[i];
            const unsigned long lastChange = this->lastChange[i];
            const bool value = this->edgeDetector[i];
            KeyEvent event(i, edges[i], holdTicks, lastChange, value);
            switch(edges[i]){
                case Edge::Posedge:
                    this->holdTicks[i] = 0;
                    this->lastChange[i] = now;
                    this->onKeyDown[i](event);
                    break;
                case Edge::Negedge:
                    this->holdTicks[i] = 0;
                    this->onKeyUp[i](event);
                    this->lastChange[i] = now;
                    break;
                case Edge::None:
                default:
                    break;
            }

        }
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::tick() {
        if(!this->active){
            return;
        }
        for(size_t i = 0; i < SIZE; ++i){
            this->holdTicks[i] += 1;
        }
        this->edgeDetector.tick();
        for(size_t i =0; i < SIZE; ++i){
            const unsigned int holdTicks = this->holdTicks[i];
            if(holdTicks == 0){
                continue;
            }
            const unsigned long lastChange = this->lastChange[i];
            const bool value = this->edgeDetector[i];
            KeyEvent event(i, Edge::None, holdTicks, lastChange, value);
            if(this->edgeDetector[i]){
                this->onHoldDown[i](event);
            }
        }
    }

}

#endif