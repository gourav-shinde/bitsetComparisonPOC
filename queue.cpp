#include<iostream>
// #include"EventClass.hpp"
#include"combinedqueue.hpp"



int main(){
    CombinedQueue queue(10);

    Event event1(1, 1, "a", "b", 1, true);
    Event event2(2, 2, "a", "b", 1, true);
    Event event3(3, 3, "a", "b", 1, true);
    Event event4(4, 4, "a", "b", 1, true);
    Event event5(5, 5, "a", "b", 1, true);
    Event event6(1, 2, "a", "b", 1, true);

    
    queue.enqueue(event1);
    queue.debug();
    // std::cout<<queue.findInsertPosition(event2)<<std::endl;
    
    // queue.debug();
    
    queue.enqueue(event2);
    queue.debug();
    
    queue.enqueue(event4);
    queue.debug();
    
    queue.enqueue(event3);
    queue.debug();
    queue.enqueue(event6);
    queue.enqueue(event5);
    queue.debug();
    queue.dequeue();
    queue.dequeue();
    queue.debug();
    queue.enqueue(event2);
    queue.debug();
    
    queue.enqueue(event4);
    queue.debug();
    

}