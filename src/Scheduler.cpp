#include "Scheduler.h"



namespace arc {

    void noop(Scheduler& scheduler){
        
    }

    Scheduler::TaskNode::TaskNode() {
        this->when = 0;
        this->action = noop;        
        this->next = NULL;
    }

    bool Scheduler::TaskNode::run(Scheduler& scheduler) {
        this->action(scheduler);
        return true;
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

    bool Scheduler::PeriodicTaskNode::run(Scheduler& scheduler) {
        this->action(scheduler);
        this->when += this->interval;
        scheduler.addTask(this);
        return false;
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
            if(taskNode->run(*this)){
                delete taskNode;
            }
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

    Scheduler::TaskSubscription Scheduler::timeout(unsigned int msWait, task_t task){
        unsigned long doWhen = millis() + msWait;
        TaskNode * node  = new Scheduler::SingleTaskNode(doWhen, task);
        this->addTask(node);
        return Scheduler::TaskSubscription(*this,node);
    }

    Scheduler::TaskSubscription Scheduler::every(unsigned int msInterval, task_t task) {
        unsigned long doWhen = millis() + msInterval;
        TaskNode * node = new Scheduler::PeriodicTaskNode(doWhen, msInterval, task);
        this->addTask(node);
        return Scheduler::TaskSubscription(*this,node);
    }

    void Scheduler::addTask(TaskNode * newNode) {
        TaskNode * node = &this->headNode;
        while(node->next != NULL && node->next->when < newNode->when){
            node = node->next;
        }
        newNode->next = node->next;
        node->next = newNode;
    }

    bool Scheduler::removeTask(TaskNode * rmNode) {
        TaskNode * node = &this->headNode;
        while(node->next != NULL && node->next != rmNode){
            node = node->next;
        }
        if(node->next == rmNode){
            node->next = rmNode->next;
            return true;
        }
        return false;
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

    Scheduler::TaskSubscription::TaskSubscription(const TaskSubscription& origin) : scheduler(origin.scheduler) {
        this->task  = origin.task;
    }

    Scheduler::TaskSubscription::TaskSubscription(Scheduler& scheduler, Scheduler::TaskNode * taskNode) : scheduler(scheduler) {
        this->task  = taskNode;
    }

    bool Scheduler::TaskSubscription::unsubscribe() {
        bool result = this->scheduler.removeTask(this->task);
        delete this->task;
        return result;
    }

}
