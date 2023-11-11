#include<iostream>
#include"EventClass.hpp"
#include"UnifiedQueue.hpp"

UnifiedQueue<Event,compareEvent> queue(10);
    
void location(Event event){
    std::cout<<queue.findInsertPosition2(event)<<std::endl;
};

int main(){
    
    

    Event event1(1, 1, "a", "b", 1, true);
    Event event2(2, 2, "a", "b", 1, true);
    Event event3(3, 3, "a", "b", 1, true);
    Event event4(4, 4, "a", "b", 1, true);
    Event event5(5, 5, "a", "b", 1, true);
    Event event6(1, 6, "a", "b", 1, true);
    Event event7(1, 7, "a", "b", 1, true);
    Event event8(1, 8, "a", "b", 1, true);
    Event event9(1, 10, "a", "b", 1, true);
    Event event10(1, 11, "a", "b", 1, true);
    Event event11(1, 9, "a", "b", 1, true);

    queue.debug();

    queue.enqueue(event1);
    queue.debug();
    
    // queue.debug();
    
    queue.enqueue(event2);
    queue.debug();
    
    queue.enqueue(event4);
    queue.debug();
    
    queue.enqueue(event3);
    queue.debug();
    queue.enqueue(event6);
    queue.debug();


    queue.enqueue(event5);
    queue.debug();

    queue.dequeue();
    queue.debug();

    queue.dequeue();
    queue.debug(); 

    queue.enqueue(event2);
    queue.debug();
    
    queue.enqueue(event4);
    queue.debug();

    queue.enqueue(event7);
    queue.debug();

    queue.enqueue(event8);
    queue.debug();

    queue.enqueue(event9);
    queue.debug();

    queue.enqueue(event11); //queue should be full
    queue.debug();


    queue.dequeue();
    queue.debug();

    queue.enqueue(event11);
    queue.debug();
    

}