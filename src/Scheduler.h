#ifndef __CIAG__BLUEFAIRY__SCHEDULER__
#define __CIAG__BLUEFAIRY__SCHEDULER__

#include "Arduino.h"
#include "List.h"

namespace ciag {
    namespace bluefairy {

        class Scheduler;
        template<typename A> class SingleTaskNode;
        template<typename A> class PeriodicTaskNode;
        template<typename A> class ExpirableTaskNode;

        class Task {
        public:
            virtual void operator()() = 0;
            ~Task();
        };

        class TaskNode {
        private:
            TaskNode(const TaskNode&);
        protected:
            bool canceled;
            Scheduler& scheduler;
            unsigned long when;
            TaskNode * next;
        public:
            friend Scheduler;
            TaskNode(Scheduler& scheduler);
            virtual bool run();
            virtual ~TaskNode();
            virtual void cancel();
        };

        class Scheduler {
        public:
            class Group;

            unsigned long tick();
            void loop();
            Scheduler() : headNode(*this) {};

            template<typename T>
            TaskNode* timeout(unsigned int msWait, T task);

            template<typename T>
            TaskNode* every(unsigned int msInterval, T task);
        
            template<typename T>
            TaskNode* every(unsigned int firstInterval, unsigned int msInterval, T task);

            template<typename T>
            TaskNode* repeat(unsigned int times, unsigned int msInterval, T task);

            template<typename T>
            TaskNode* repeat(unsigned int times, unsigned int firstInterval, unsigned int msInterval, T task);

            Group group();

            void debug(Stream& stream);
            virtual ~Scheduler();


            TaskNode* addTask(TaskNode * taskNode);
            bool removeTask(TaskNode * taskNode);

            class Group {
            public:
                Group(Scheduler& scheduler);
                Group& cancel();

                template<typename T>
                Group& timeout(unsigned int msWait, T task);

                template<typename T>
                Group&  every(unsigned int msInterval, T task);

                template<typename T>
                Group& every(unsigned int firstInterval, unsigned int msInterval, T task);

                template<typename T>
                Group& repeat(unsigned int times, unsigned int msInterval, T task);

                template<typename T>
                Group& repeat(unsigned int times, unsigned int firstInterval, unsigned int msInterval, T task);

            protected:
                Scheduler& scheduler;
                List<TaskNode*> tasks;
            };

        protected:
            TaskNode headNode;
        private:
            Scheduler(const Scheduler&);
        };

        template<typename T>
        TaskNode* Scheduler::timeout(unsigned int msWait, T task) {
            unsigned long doWhen = millis() + msWait;
            TaskNode * node  = new SingleTaskNode<T>(*this, doWhen, task);
            return this->addTask(node);
        }

        template<typename T>
        TaskNode* Scheduler::every(unsigned int msInterval, T task) {
            return this->every(msInterval, msInterval, task);
        }

        template<typename T>
        TaskNode* Scheduler::every(unsigned int firstInterval, unsigned int msInterval, T task) {
            unsigned long doWhen = millis() + firstInterval;
            TaskNode * node = new PeriodicTaskNode<T>(*this, doWhen, msInterval, task);
            return this->addTask(node);
        }

        template<typename T>
        TaskNode* Scheduler::repeat(unsigned int times, unsigned int msInterval, T task) {
            return this->repeat(times, msInterval, msInterval, task);
        }

        template<typename T>
        TaskNode* Scheduler::repeat(unsigned int times, unsigned int firstInterval, unsigned int msInterval, T task) {
            unsigned long doWhen = millis() + firstInterval;
            TaskNode * node = new ExpirableTaskNode<T>(*this, times, doWhen, msInterval, task);
            return this->addTask(node);
        }

        template<typename T>
        Scheduler::Group& Scheduler::Group::timeout(unsigned int msWait, T task) {
            this->tasks.add(scheduler.timeout(msWait,task));
            return *this;
        }

        template<typename T>
        Scheduler::Group&  Scheduler::Group::every(unsigned int msInterval, T task) {
            return this->every(msInterval, msInterval, task);
        }

        template<typename T>
        Scheduler::Group& Scheduler::Group::every(unsigned int firstInterval, unsigned int msInterval, T task) {
            this->tasks.add(scheduler.every(firstInterval, msInterval, task));
            return *this;
        }

        template<typename T>
        Scheduler::Group& Scheduler::Group::repeat(unsigned int times, unsigned int msInterval, T task) {
            return this->repeat(times, msInterval, msInterval, task);
        }

        template<typename T>
        Scheduler::Group& Scheduler::Group::repeat(unsigned int times, unsigned int firstInterval, unsigned int msInterval, T task) {
            this->tasks.add(scheduler.repeat(times, firstInterval,msInterval,task));
            return *this;
        }


        template<typename A>
        class BaseTaskNode : public TaskNode{
        protected:
            A action;
        public:
            BaseTaskNode(Scheduler& scheduler, A action) : TaskNode(scheduler), action(action) {};

            virtual bool run() {
                this->action();
                return true;
            }
        };

        template<typename A>
        class SingleTaskNode: public BaseTaskNode<A> {
        public:
            SingleTaskNode(Scheduler& scheduler, unsigned long when, A action) : BaseTaskNode<A>(scheduler, action) {
                this->when = when;
                this->next = NULL;
            }
        };

        template<typename A>
        class PeriodicTaskNode : public BaseTaskNode<A> {
        public:
            unsigned int interval;

            virtual bool run() {
                this->action();
                this->when += this->interval;
                this->scheduler.addTask(this);
                return false;
            }

            PeriodicTaskNode(Scheduler& scheduler, unsigned long when, unsigned int interval, A action) : BaseTaskNode<A>(scheduler, action) {
                this->when = when;
                this->interval = interval;
            }
        };

        template<typename A>
        class ExpirableTaskNode : public PeriodicTaskNode<A>{
        public:
            unsigned int counter;
            unsigned int times;
            virtual bool run() {
                this->action();
                this->counter += 1;
                this->when += this->interval;
                bool repeat = this->counter < this->times;
                if(repeat) {
                    this->scheduler.addTask(this);
                }
                return !repeat;
            }
            ExpirableTaskNode(Scheduler& scheduler, unsigned int times, unsigned long when, unsigned int interval, A action) : PeriodicTaskNode<A>(scheduler, when, interval, action) {
                this->counter = 0;
                this->times = times;
            }
        };

    }
}

#endif