#ifndef __CIAG__BLUEFAIRY__KEYBOARD__
#define __CIAG__BLUEFAIRY__KEYBOARD__

#include "EdgeDetector.h"

namespace ciag {
    namespace bluefairy {

        class KeyEvent {
        public:
            size_t key;
            Edge edge;
            unsigned int holdTicks;
            unsigned long lastChange;
            bool value;

            KeyEvent(size_t key, Edge edge, unsigned int holdTicks, unsigned long lastChange, bool value);

            unsigned int holdTime() const;

        };

        class KeyAction {
        public:
            virtual void perform(const KeyEvent&) =0;
            virtual ~KeyAction() {};
        };

        class NullAction : public KeyAction {
        public:
            void perform(const KeyEvent&);
        };

        template<typename T>
        class TemplateKeyAction : public KeyAction {
        protected:
            T onPerform;
        public:
            TemplateKeyAction(T onPerform);
            void perform(const KeyEvent& keyEvent);
        };

        template<typename T>
        TemplateKeyAction<T>::TemplateKeyAction(T onPerform) : onPerform(onPerform) {

        }

        template<typename T>
        void TemplateKeyAction<T>::perform(const KeyEvent& keyEvent){
            this->onPerform(keyEvent);
        }

        class KeyActionHolder {
        public:
            KeyActionHolder();
            ~KeyActionHolder();
            void clear();
            void operator()(const KeyEvent&);
            template<typename T>
            KeyActionHolder& operator=(T action);
        protected:
            KeyAction * keyAction;
        };

        template<typename T>
        KeyActionHolder&  KeyActionHolder::operator=(T action){
            this->clear();
            this->keyAction = new TemplateKeyAction<T>(action);
            return *this;
        }

        template<size_t SIZE>
        class Keyboard {
        public:
            Keyboard(const byte* const pins);
            void tick();
            unsigned long lastChange[SIZE];
            unsigned int holdTicks[SIZE];
            KeyActionHolder onKeyDown[SIZE];
            KeyActionHolder onKeyUp[SIZE];
            KeyActionHolder onHoldDown[SIZE];
            void edgeTrigger(const Edge* const edges);
            void operator()(const Edge* const edges);
            void clear();
            bool isActive() const { return this->active; }
            void turnOff();
            void turnOn();
        protected:
            bool active = true;
            EdgeDetector<SIZE, Keyboard&> edgeDetector;
        private:
            Keyboard(const Keyboard<SIZE>&);
        };

        template<size_t SIZE>
        Keyboard<SIZE>::Keyboard(const byte* const pins): edgeDetector(pins,*this){
            this->clear();
        }

        template<size_t SIZE>
        void Keyboard<SIZE>::clear() {
            for(size_t i =0;i<SIZE;++i){
                this->holdTicks[i] = 0;
                this->lastChange[i] = 0;
                this->onKeyDown[i].clear();
                this->onKeyUp[i].clear();
                this->onHoldDown[i].clear();
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
}

#endif