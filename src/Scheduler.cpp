#include "Scheduler.h"



namespace arc {

    void noop(Scheduler& scheduler){
        
    }

    Scheduler::TaskNode::TaskNode() {
        this->when = 0;
        this->action = noop;        
        this->next = NULL;
    }

    void Scheduler::TaskNode::run(Scheduler& scheduler) {
        this->action(scheduler);
    }

    Scheduler::SingleTaskNode::SingleTaskNode(unsigned long when, task_t action) {
        this->when = when;
        this->action = action;
        this->next = NULL;
    }

    Scheduler::PeriodicTaskNode::PeriodicTaskNode(unsigned long when, unsigned int interval, task_t action) {
        this->when = when;
        this->interval = interval;
        this->action = action;
    }

    Scheduler::PeriodicTaskNode* Scheduler::PeriodicTaskNode::makeNext() const {
        return new Scheduler::PeriodicTaskNode(this->when + this->interval, this->interval, this->action);
    }

    void Scheduler::PeriodicTaskNode::run(Scheduler& scheduler) {
        TaskNode * nextIteration = this->makeNext();
        scheduler.addTask(nextIteration);
        this->action(scheduler);
    }

    Scheduler::~Scheduler() {
        TaskNode * taskNode = this->headNode.next;
        while(taskNode != NULL) {
            TaskNode * next = taskNode->next;
            delete taskNode;
            taskNode = next;
        }
    }

    unsigned long Scheduler::tick() {
        unsigned long currentTime = millis();
        TaskNode * taskNode = this->headNode.next;
        while(taskNode != NULL) {
            if(taskNode->when > currentTime) {
                this->headNode.next = taskNode;
                return taskNode->when - currentTime;
            }
            taskNode->run(*this);
            TaskNode * executed = taskNode;
            taskNode = taskNode->next;
            delete executed;
            currentTime = millis();
        }
        this->headNode.next = NULL;
        return 0;
    }

    void Scheduler::loop() {
        while(this->headNode.next != NULL){
            const unsigned long wait = this->tick();
            delay(wait);
        }
    }

    void Scheduler::timeout(unsigned int msWait, task_t task){
        unsigned long doWhen = millis() + msWait;
        TaskNode * node  = new Scheduler::SingleTaskNode(doWhen, task);
        this->addTask(node);
    }

    void Scheduler::every(unsigned int msInterval, task_t task) {
        unsigned long doWhen = millis() + msInterval;
        TaskNode * node = new Scheduler::PeriodicTaskNode(doWhen, msInterval, task);
        this->addTask(node);
    }

    void Scheduler::addTask(TaskNode * newNode) {
        TaskNode * node = &this->headNode;
        while(node->next != NULL && node->next->when < newNode->when){
            node = node->next;
        }
        newNode->next = node->next;
        node->next = newNode;
    }

    void Scheduler::debug(Stream& stream){
        TaskNode * taskNode = this->headNode.next;
        while(taskNode != NULL) {
            stream.print(taskNode->when);
            stream.print(" ");
            taskNode = taskNode->next;
        }
        stream.println("");
    }

}
