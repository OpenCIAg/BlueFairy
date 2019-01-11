#ifndef __ARC_SCHEDULER__
#define __ARC_SCHEDULER__

#include "Arduino.h"

namespace arc {

    class Scheduler;
    class Subscription;

    typedef void (*task_t)(Scheduler&);

    class Subscription {
    public:
        virtual bool unsubscribe() = 0;
        virtual ~Subscription(){};
    };

    class Scheduler {
    public:
        class TaskNode;
        class SingleTaskNode;
        class PeriodicTaskNode;
        class TaskSubscription;
        unsigned long tick();
        void loop();
        TaskSubscription timeout(unsigned int msWait, task_t task);
        TaskSubscription every(unsigned int msPeriod, task_t task);
        void debug(Stream& stream);
        virtual ~Scheduler();
        class TaskNode {
        public:
            task_t action;
            unsigned long when;
            virtual bool run(Scheduler& scheduler);
            TaskNode * next;
            TaskNode();
            virtual ~TaskNode(){};
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
            virtual bool run(Scheduler& scheduler);
            PeriodicTaskNode(unsigned long when, unsigned int interval, task_t action);
        };
        class TaskSubscription : public Subscription{        
        public:
            TaskSubscription(Scheduler&,TaskNode*);
            TaskSubscription(const TaskSubscription&);
            bool unsubscribe();
        protected:
            Scheduler& scheduler;
            TaskNode * task;
        };
    protected:
        TaskNode headNode;
        void addTask(TaskNode * taskNode);
        bool removeTask(TaskNode * taskNode);
    };


}

#endif