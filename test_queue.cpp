// write tests using gtest to test UnifiedQUeue

#include <gtest/gtest.h>
#include "UnifiedQueue.hpp"
// #include "EventClass.hpp"
#include <thread>
#include <chrono>
#include "randomEventGenerator.hpp"

RandomEventGenerator r;
// test for isEmpty
TEST(UnifiedQueueTest, Markers) {
    UnifiedQueue<Event, compareEvent> queue(10);
    //init condition check
    EXPECT_EQ(queue.getActiveStart(), 0);
    EXPECT_EQ(queue.getUnprocessedStart(), 0);
    EXPECT_EQ(queue.getFreeStart(), 0);
    EXPECT_EQ(queue.getFreeSign(), 0);
    EXPECT_EQ(queue.getUnprocessedSign(), 0);
    //set testing
    queue.setActiveStart(5);
    queue.setUnprocessedStart(2);
    queue.setFreeStart(3);
    EXPECT_EQ(queue.getActiveStart(), 5);
    EXPECT_EQ(queue.getUnprocessedStart(), 2);
    EXPECT_EQ(queue.getFreeStart(), 3);
    //set sign
    queue.setFreeSign(1);
    queue.setUnprocessedSign(1);
    EXPECT_EQ(queue.getFreeSign(), 1);
    EXPECT_EQ(queue.getUnprocessedSign(), 1);

}

//tests for local markers modifier
TEST(UnifiedQueueTest, Markers2) {
    UnifiedQueue<Event, compareEvent> queue(10);
    uint32_t local_marker = 0;
    queue.setActiveStartMarker(local_marker, 5);
    EXPECT_EQ(queue.ActiveStart(local_marker), 5);
    queue.setUnprocessedSignMarker(local_marker, 1);
    EXPECT_EQ(queue.ActiveStart(local_marker), 5);
    EXPECT_EQ(queue.UnProcessedSign(local_marker), 1);
    queue.setUnprocessedStartMarker(local_marker, 2);
    EXPECT_EQ(queue.ActiveStart(local_marker), 5);
    EXPECT_EQ(queue.UnProcessedSign(local_marker), 1);
    EXPECT_EQ(queue.UnprocessedStart(local_marker), 2);
    queue.setFreeSignMarker(local_marker, 1);
    EXPECT_EQ(queue.ActiveStart(local_marker), 5);
    EXPECT_EQ(queue.UnProcessedSign(local_marker), 1);
    EXPECT_EQ(queue.UnprocessedStart(local_marker), 2);
    EXPECT_EQ(queue.FreeSign(local_marker), 1);
    queue.setFreeStartMarker(local_marker, 3);
    EXPECT_EQ(queue.ActiveStart(local_marker), 5);
    EXPECT_EQ(queue.UnProcessedSign(local_marker), 1);
    EXPECT_EQ(queue.UnprocessedStart(local_marker), 2);
    EXPECT_EQ(queue.FreeSign(local_marker), 1);
    EXPECT_EQ(queue.FreeStart(local_marker), 3);

}

TEST(UnifiedQueueTest, isEmptyTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    queue.debug();
    EXPECT_EQ(queue.isEmpty(), true);
    queue.enqueue(r.getEvent());
    queue.debug();
    EXPECT_EQ(queue.isEmpty(), false);
    queue.dequeue();
    queue.increamentActiveStart();//this will increament active start to emulate fossile collection.
    queue.increamentActiveStart();
    std::cout<<"hmmm"<<queue.isEmpty()<<std::endl;
    EXPECT_EQ(queue.isEmpty(), true);
    queue.debug();

}

// test for isFull
TEST(UnifiedQueueTest, isFullTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    EXPECT_EQ(queue.isFull(), false);
    for(int i = 0; i < 10; i++){
        queue.enqueue(r.getEvent());
    }
    EXPECT_EQ(queue.isFull(), true);
}

// test for capacity
TEST(UnifiedQueueTest, capacityTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    EXPECT_EQ(queue.capacity(), 10);
}

// test for size
TEST(UnifiedQueueTest, sizeTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    EXPECT_EQ(queue.size(), 0);
    queue.enqueue(r.getEvent());
    queue.enqueue(r.getEvent());
    EXPECT_EQ(queue.size(), 2);
}

// test for enqueue and dequeue
TEST(UnifiedQueueTest, enqueueTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    Event e1 = r.getEvent();
    Event e2 = r.getEvent();
    queue.enqueue(e1);
    queue.enqueue(e2);//change the functions so that they return bool
}



// test for findInsertPosition
// TEST(UnifiedQueueTest, PositionTest){
//     UnifiedQueue<Event, compareEvent> queue(10);
//     int pos = queue.findInsertPosition(Event (1, 1, "a", "b", 1, true),queue.getActiveStart(),queue.getFreeStart());
//     EXPECT_EQ(pos, 0);
//     queue.enqueue(Event(1, 1, "a", "b", 1, true));
//     pos = queue.findInsertPosition(Event(3, 3, "a", "b", 1, true),queue.getActiveStart(),queue.getFreeStart());
//     EXPECT_EQ(pos, 1);
//     queue.enqueue(Event(3, 3, "a", "b", 1, true));
//     pos = queue.findInsertPosition(Event(2, 2, "a", "b", 1, true),queue.getActiveStart(),queue.getFreeStart());
//     EXPECT_EQ(pos, 1);
//     queue.enqueue(Event(2, 2, "a", "b", 1, true)); //let this be here if u wanna add more cases
//     // add rotation cases
//     queue.enqueue(Event(4, 4, "a", "b", 1, true));
//     queue.enqueue(Event(5, 5, "a", "b", 1, true));
//     queue.enqueue(Event(6, 6, "a", "b", 1, true));
//     queue.enqueue(Event(7, 7, "a", "b", 1, true));
    
//     queue.enqueue(Event(8, 8, "a", "b", 1, true));
//     queue.enqueue(Event(9, 9, "a", "b", 1, true));
//     queue.enqueue(Event(10, 10, "a", "b", 1, true));
//     EXPECT_EQ(queue.isFull(), 1);
//     queue.debug();

//     queue.dequeue();
//     queue.debug();
//     queue.increamentActiveStart(); ; //these two lines replicate fossil collection and event processing
//     queue.dequeue();
//     queue.debug();
//     queue.increamentActiveStart(); ;

//     pos = queue.findInsertPosition(Event(11, 11, "a", "b", 1, true),queue.getActiveStart(),queue.getFreeStart()); //this should be inserted at pos 0
//     EXPECT_EQ(pos, 0);
//     queue.enqueue(Event(11, 11, "a", "b", 1, true));
//     pos = queue.findInsertPosition(Event(11, 10, "a", "b", 2, true),queue.getActiveStart(),queue.getFreeStart()); //this should be inserted at pos 0
//     EXPECT_EQ(pos, 0);
// }

//not on zero
TEST(UnifiedQueueTest, SizeTest){
    UnifiedQueue<Event, compareEvent> queue(4);
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.debug();
    EXPECT_EQ(queue.size(), 4); //00-4  markers
    queue.dequeue();
    queue.increamentActiveStart();
    EXPECT_EQ(queue.size(), 3); //110  markers
    queue.debug();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.debug();
    EXPECT_EQ(queue.size(), 3); //1-40 markers
    queue.enqueue(Event(5, 5, "a", "b", 1, true)); //10-1 markers
    queue.debug();
    queue.increamentActiveStart();
    queue.debug();
    queue.increamentActiveStart();
    queue.increamentActiveStart();
    queue.debug(); //-401 markers
    queue.dequeue();
    queue.debug();
    queue.increamentActiveStart();
    queue.debug();
    EXPECT_EQ(queue.size(), 0); //0-40 markers
    EXPECT_EQ(queue.isEmpty(), true);
    queue.increamentActiveStart();
    queue.debug();
    queue.dequeue();
    queue.debug();
    // test for -x -x x condition this should be empty, this is stopped by isEmpty() function
    queue.increamentActiveStart();
    queue.debug(); 
}


// TESTING ABA
void enqueue(UnifiedQueue<Event, compareEvent> *queue){
    sleep(1);
    queue->enqueue(Event(1, 1, "a", "b", 1, true));
    queue->enqueue(Event(2, 2, "a", "b", 1, true));
    queue->enqueue(Event(3, 3, "a", "b", 1, true));
    queue->enqueue(Event(4, 4, "a", "b", 1, true));
    queue->enqueue(Event(5, 5, "a", "b", 1, true));

}

void enqueue2(UnifiedQueue<Event, compareEvent> *queue){
    queue->enqueue(Event(10, 10, "a", "b", 1, true));
    // queue->dequeue();
    sleep(1);
    queue->enqueue(Event(11, 11, "a", "b", 1, true));
    queue->enqueue(Event(13, 13, "a", "b", 1, true));
    queue->enqueue(Event(14, 14, "a", "b", 1, true));
    
    // queue->dequeue();
}

TEST(UnifiedQueueTest, ABA1){
    UnifiedQueue<Event, compareEvent> queue(10);
    queue.enqueue(Event(12, 12, "a", "b", 1, true)); //prepopulate
    std::thread t1(enqueue, &queue);
    std::thread t2(enqueue2, &queue);
    t1.join();
    t2.join();
    queue.debug();
    EXPECT_EQ(queue.size(), 10);
}

void dequeue(UnifiedQueue<Event, compareEvent> *queue){
    sleep(1);
    queue->dequeue();
    queue->dequeue();
    queue->dequeue();
    queue->dequeue();
    queue->dequeue();
}

void dequeue2(UnifiedQueue<Event, compareEvent> *queue){
    queue->dequeue();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    queue->dequeue();
    queue->dequeue();
    queue->dequeue();
}



TEST(UnifiedQueueTest, ABA2){
    UnifiedQueue<Event, compareEvent> queue(30);
    //prepopulate
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.enqueue(Event(5, 5, "a", "b", 1, true));
    queue.enqueue(Event(6, 6, "a", "b", 1, true));
    queue.enqueue(Event(7, 7, "a", "b", 1, true));
    queue.enqueue(Event(8, 8, "a", "b", 1, true));
    queue.enqueue(Event(9, 9, "a", "b", 1, true));
    queue.enqueue(Event(10, 10, "a", "b", 1, true));
    queue.enqueue(Event(11, 11, "a", "b", 1, true));
    queue.enqueue(Event(12, 12, "a", "b", 1, true));
    queue.enqueue(Event(21, 21, "a", "b", 1, true));
    queue.enqueue(Event(22, 22, "a", "b", 1, true));
    queue.enqueue(Event(23, 23, "a", "b", 1, true));
    queue.enqueue(Event(24, 24, "a", "b", 1, true));
    
    std::thread t1(enqueue2, &queue);
    std::thread t2(dequeue, &queue);
    std::thread t3(dequeue2, &queue);
    t2.join();
    t1.join();
    t3.join();
    queue.debug();
    EXPECT_EQ(queue.getUnprocessedStart(), 10);
    EXPECT_EQ(queue.getFreeStart(), 20);
}

void fossil(UnifiedQueue<Event, compareEvent> *queue){
    sleep(1);
    queue->increamentActiveStart();
    queue->increamentActiveStart();
    queue->increamentActiveStart();
    queue->increamentActiveStart();
    queue->increamentActiveStart();
}

void fossil2(UnifiedQueue<Event, compareEvent> *queue){
    queue->increamentActiveStart();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->increamentActiveStart();
    queue->increamentActiveStart();
    queue->increamentActiveStart();
    queue->increamentActiveStart();
}

TEST(UnifiedQueue,ABA3){
    UnifiedQueue<Event, compareEvent> queue(30);
    //prepopulate *16
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.enqueue(Event(5, 5, "a", "b", 1, true));
    queue.enqueue(Event(6, 6, "a", "b", 1, true));
    queue.enqueue(Event(7, 7, "a", "b", 1, true));
    queue.enqueue(Event(8, 8, "a", "b", 1, true));
    queue.enqueue(Event(9, 9, "a", "b", 1, true));
    queue.enqueue(Event(10, 10, "a", "b", 1, true));
    queue.enqueue(Event(11, 11, "a", "b", 1, true));
    queue.enqueue(Event(12, 12, "a", "b", 1, true));
    queue.enqueue(Event(21, 21, "a", "b", 1, true));
    queue.enqueue(Event(22, 22, "a", "b", 1, true));
    queue.enqueue(Event(23, 23, "a", "b", 1, true));
    queue.enqueue(Event(24, 24, "a", "b", 1, true));
    // dequeue *10
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    
    std::thread t1(enqueue2, &queue);
    std::thread t2(fossil, &queue);
    std::thread t3(fossil2, &queue);
    t2.join();
    t1.join();
    t3.join();
    queue.debug();
    EXPECT_EQ(queue.getUnprocessedStart(), 10);
    EXPECT_EQ(queue.getFreeStart(), 20);
    EXPECT_EQ(queue.getActiveStart(), 10);
}

TEST(UnifiedQueue,ThreadTest){
    UnifiedQueue<Event, compareEvent> queue(30);
    //prepopulate *16
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.enqueue(Event(5, 5, "a", "b", 1, true));
    queue.enqueue(Event(6, 6, "a", "b", 1, true));
    queue.enqueue(Event(7, 7, "a", "b", 1, true));
    queue.enqueue(Event(8, 8, "a", "b", 1, true));
    queue.enqueue(Event(9, 9, "a", "b", 1, true));
    queue.enqueue(Event(10, 10, "a", "b", 1, true));
    queue.enqueue(Event(11, 11, "a", "b", 1, true));
    queue.enqueue(Event(12, 12, "a", "b", 1, true));
    queue.enqueue(Event(21, 21, "a", "b", 1, true));
    queue.enqueue(Event(22, 22, "a", "b", 1, true));
    queue.enqueue(Event(23, 23, "a", "b", 1, true));
    queue.enqueue(Event(24, 24, "a", "b", 1, true));
    
    
    std::thread t1(enqueue2, &queue);
    std::thread t2(enqueue, &queue);
    std::thread t3(fossil2, &queue); //doesnt change a thing
    t2.join();
    t1.join();
    t3.join();
    queue.debug();
    EXPECT_EQ(queue.getUnprocessedStart(), 0);
    EXPECT_EQ(queue.getFreeStart(), 25);
    EXPECT_EQ(queue.getActiveStart(), 0);
    dequeue(&queue);
    dequeue2(&queue);

    std::thread t4(dequeue, &queue);
    std::thread t5(dequeue2, &queue);
    std::thread t6(dequeue, &queue); 
    t4.join();
    t5.join();
    t6.join();
    dequeue(&queue); //unprocess queue is empty
    EXPECT_EQ(queue.getUnprocessedStart(), 25);
    EXPECT_EQ(queue.getUnprocessedSign(), 1);
    EXPECT_EQ(queue.getFreeStart(), 25);

    std::thread t7(enqueue, &queue);
    std::thread t8(enqueue2, &queue);
    std::thread t9(fossil, &queue); 
    std::thread t10(fossil2, &queue); 
    t7.join();
    t8.join();
    t9.join();
    t10.join();

    EXPECT_EQ(queue.getUnprocessedStart(), 25);
    EXPECT_EQ(queue.getUnprocessedSign(), 0);
    EXPECT_EQ(queue.getFreeStart(), 4);
    EXPECT_EQ(queue.getActiveStart(), 10);

    queue.debug();

}


void enqueue3(UnifiedQueue<Event, compareEvent> *queue){
    queue->enqueue(Event(11, 11, "a", "b", 1, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->enqueue(Event(12, 12, "a", "b", 1, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->enqueue(Event(13, 13, "a", "b", 1, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->enqueue(Event(14, 14, "a", "b", 1, true));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));


}
void dequeue3(UnifiedQueue<Event, compareEvent> *queue){
    queue->dequeue();
    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    queue->dequeue();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void Find(UnifiedQueue<Event, compareEvent> *queue){
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_EQ(queue->find(Event(3, 3, "a", "b", 1, true)), queue->FindStatus::ACTIVE);
    queue->debug();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(queue->find(Event(5, 5, "a", "b", 1, true)), queue->FindStatus::ACTIVE);
    queue->debug();
    EXPECT_EQ(queue->find(Event(11, 11, "a", "b", 1, true)), queue->FindStatus::UNPROCESSED);
    queue->debug();
    EXPECT_EQ(queue->find(Event(1, 1, "a", "b", 1, true)), queue->FindStatus::NOTFOUND);
    //will invalidate the event

}

void fossile3(UnifiedQueue<Event, compareEvent> *queue){
    queue->increamentActiveStart();
}


TEST(UnifiedQueue, FindTests){
    UnifiedQueue<Event, compareEvent> queue(30);
    //prepopulate *16
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.enqueue(Event(5, 5, "a", "b", 1, true));
    queue.enqueue(Event(6, 6, "a", "b", 1, true));
    queue.enqueue(Event(7, 7, "a", "b", 1, true));
    queue.enqueue(Event(8, 8, "a", "b", 1, true));
    queue.enqueue(Event(9, 9, "a", "b", 1, true));
    queue.enqueue(Event(10, 10, "a", "b", 1, true));
    queue.debug();
    EXPECT_EQ(queue.find(Event(1, 1, "a", "b", 1, true)), queue.FindStatus::UNPROCESSED);
    queue.dequeue();
    EXPECT_EQ(queue.find(Event(1, 1, "a", "b", 1, true)), queue.FindStatus::ACTIVE);
    queue.increamentActiveStart();
    EXPECT_EQ(queue.find(Event(1, 1, "a", "b", 1, true)), queue.FindStatus::NOTFOUND);
    std::thread t1(enqueue3, &queue);
    std::thread t2(dequeue3, &queue);
    std::thread t3(Find, &queue); 
    std::thread t4(fossile3, &queue);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    queue.debug();
    queue.dequeue();
    queue.debug();
    queue.dequeue();//this dequeues 11, but its invalid so it gets next element
    queue.debug(); 
    EXPECT_EQ(queue.getUnprocessedStart(), 12);
    //dequeue and find should be on single thread so this test is valid
}


TEST(UnfiedQueue, fixPosition){
    UnifiedQueue<Event, compareEvent> queue(30);
    //prepopulate *16
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.enqueue(Event(9, 9, "a", "b", 1, true));
    queue.enqueue(Event(7, 7, "a", "b", 1, true));
    queue.enqueue(Event(5, 5, "a", "b", 1, true));
    queue.enqueue(Event(6, 6, "a", "b", 1, true));
    queue.enqueue(Event(8, 8, "a", "b", 1, true));
    queue.enqueue(Event(10, 10, "a", "b", 1, true));

    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    compareEvent obj;
    queue.debug();
    Event e =queue.dequeue();
    Event e2 = queue.getPreviousUnprocessedEvent();
    std::cout<<"e"<<e.receiveTime_<<std::endl;
    std::cout<<"e2"<<e2.receiveTime_<<std::endl; 
    if(obj(e,e2)){ //this is rollback
        std::cout<<"yes"<<std::endl;
        queue.fixPosition();
    }
    queue.debug();
    queue.increamentActiveStart();
    queue.increamentActiveStart();// so 3 start
    queue.find(Event(7, 7, "a", "b", 1, true)); //this will invalidate 7, so 5 will be placed in front of 7
    Event check = queue.dequeue(); //will return 9
    EXPECT_EQ(check.receiveTime_, 9);
    Event e3 =queue.dequeue();
    Event e4 = queue.getPreviousUnprocessedEvent();
    std::cout<<"e3"<<e.receiveTime_<<std::endl;
    std::cout<<"e4"<<e2.receiveTime_<<std::endl; 
    if(obj(e,e2)){ //this is rollback
        std::cout<<"yes"<<std::endl;
        queue.fixPosition();//this should place 6 before 5 becoz
    }
    queue.debug();



}


int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}


//discuss about the case where
//freeStart_ == activeStart_ == unprocessedStart_ (queue is empty)
//freeStart_ == activeStart_ == unprocessedStart_ (if we do not process any events)(queue is full)

// size 8

// 444 _Empty
// 445
// 446
// 447
// 440
// 441
// 442
// 453
// 46-4 _Full   
// 47-4
// 40-4
// 41-4
// 42-4
// 43-4
// 44-4 _Full



// now
// in  enqueue.
// if abs(free)!= active queue is not full and freeStart=abs freeStart.


//unprocessed changes
//freeStart_ == activeStart_ != unprocessedStart_ (certain)


// if unprocessed start becomes == freestart then -ve it, to show it is empty.
// if active start becomes == unprocessed stat then -ve it to show it is empty


// 330

// 0->1
// a3 compare 1, if equal t

// save free to locale_t
// increament local free, compare and swap against activeStart, if they are equal then -x 

// no rotation
// 000
// 003
// 023
// 223


// rotation
// 220
// 221
// 22-2 queue is full and full of unprocessed
// 23-2
// 20-2
// 21-2
// 2-2-2 queue is full and full of processed.


