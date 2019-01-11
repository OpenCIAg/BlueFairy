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
        this->action(scheduler);
        scheduler.addTask(nextIteration);
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
        while(this->headNode.next != NULL) {
            if(this->headNode.next->when > currentTime) {
                return this->headNode.next->when - currentTime;
            }
            TaskNode * taskNode = this->headNode.next;
            this->headNode.next = taskNode->next;
            taskNode->run(*this);
            delete taskNode;
            currentTime = millis();
        }
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
        stream.println("Scheduler::debug");
        stream.print("Now: ");
        stream.println(millis());
        stream.print("Scheduler Tasks: ");
        TaskNode * taskNode = this->headNode.next;
        while(taskNode != NULL) {
            stream.print(taskNode->when);
            stream.print(" ");
            taskNode = taskNode->next;
        }
        stream.println("");
    }

}
