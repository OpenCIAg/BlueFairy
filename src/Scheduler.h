#ifndef __ARC_SCHEDULER__
#define __ARC_SCHEDULER__

#include "Arduino.h"

namespace arc {

    class Scheduler;

    typedef void (*task_t)(Scheduler&);

    class Scheduler {
    public:
        unsigned long tick();
        void loop();
        void timeout(unsigned int msWait, task_t task);
        void every(unsigned int msPeriod, task_t task);
        void debug(Stream& stream);
        virtual ~Scheduler();
        class TaskNode {
        public:
            task_t action;
            unsigned long when;
            virtual void run(Scheduler& scheduler);
            TaskNode * next;
            TaskNode();
        };
        class SingleTaskNode: public TaskNode {
        public:
            SingleTaskNode(unsigned long when, task_t action);
        };
        class PeriodicTaskNode : public TaskNode {
        protected:
            PeriodicTaskNode* makeNext() const;
        public:
            unsigned int interval;
            virtual void run(Scheduler& scheduler);
            PeriodicTaskNode(unsigned long when, unsigned int interval, task_t action);
        };
    protected:
        TaskNode headNode;
        void addTask(TaskNode * taskNode);
    };


}

#endif