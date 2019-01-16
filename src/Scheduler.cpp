#include "Scheduler.h"



namespace arc {

    TaskNode::TaskNode(Scheduler& scheduler): scheduler(scheduler) {
        this->when = 0;
        this->next = NULL;
    }

    TaskNode::~TaskNode() {
        this->scheduler.removeTask(this);
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
            if(taskNode->run()){
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

    TaskNode * Scheduler::addTask(TaskNode * newNode) {
        TaskNode * node = &this->headNode;
        while(node->next != NULL && node->next->when < newNode->when){
            node = node->next;
        }
        newNode->next = node->next;
        node->next = newNode;
        return newNode;
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

}
