#include<iostream>
#include<thread>
#include"randomEventGenerator.hpp"
#include"UnifiedQueue.hpp"

void do_work(UnifiedQueue<Event, compareEvent> *queue){
    RandomEventGenerator r;
    for(int i=0;i<30;i++){
        queue->enqueue(r.getEvent());
    }
}

void do_random_work(UnifiedQueue<Event, compareEvent> *queue){
    // generate random number
    // if random number is even, enqueue
    // if random number is odd, dequeue
    RandomEventGenerator r;
    for(int i=0;i<50;i++){
        int random = r.generateRandomUint8();
        if(random%2==0){
            queue->enqueue(r.getEvent());
        }
        else{
            queue->dequeue(); //to emulate process event
            queue->increamentActiveStart_Index() ;//to emulate fossile collection
        }
    }
    
}

int main(){
    UnifiedQueue<Event, compareEvent> queue(64);
    std::thread t1(do_work, &queue);
    std::thread t2(do_work, &queue);
    t1.join();
    t2.join();
    queue.debug();
    
    std::thread t3(do_random_work, &queue);
    std::thread t4(do_random_work, &queue);
    t3.join();
    t4.join();
    queue.debug();
}