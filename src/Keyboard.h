#ifndef __ARC__KEYBOARD__
#define __ARC__KEYBOARD__
#include "EdgeDetector.h"

namespace arc {

    typedef void (*key_action_t)();
    key_action_t noop = [](){};

    template<size_t SIZE>
    class Keyboard {
    public:
        Keyboard(const byte* const pins);
        void tick();
        key_action_t onKeyDown[SIZE];
        key_action_t onKeyUp[SIZE];
        void edgeTrigger(const Edge* const edges);
        void operator()(const Edge* const edges);
    protected:
        EdgeDetector<SIZE, Keyboard&> edgeDetector;
    };

    template<size_t SIZE>
    Keyboard<SIZE>::Keyboard(const byte* const pins): edgeDetector(pins,*this){
        for(size_t i =0;i<SIZE;++i){
            this->onKeyDown[i] = noop;
            this->onKeyUp[i] = noop;
        }
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::operator()(const Edge* const edges){
        this->edgeTrigger(edges);
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::edgeTrigger(const Edge* const edges){
        for(size_t i =0;i<SIZE;i+=1){
            switch(edges[i]){
                case Edge::Negedge:
                    this->onKeyDown[i]();
                    break;
                case Edge::Posedge:
                    this->onKeyUp[i]();
                    break;
                case Edge::None:
                default:
                    break;
            }
        }
    }

    template<size_t SIZE>
    void Keyboard<SIZE>::tick() {
        this->edgeDetector.tick();
    }

}

#endif