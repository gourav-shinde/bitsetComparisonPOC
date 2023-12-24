// write tests using gtest to test UnifiedQUeue

#include <gtest/gtest.h>
#include "UnifiedQueue.hpp"
#include "randomEventGenerator.hpp"
#include <thread>


RandomEventGenerator r;
// test for isEmpty
TEST(UnifiedQueueTest, isEmptyTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    queue.debug();
    EXPECT_EQ(queue.isEmpty(), true);
    queue.enqueue(r.getEvent());
    queue.debug();
    EXPECT_EQ(queue.isEmpty(), false);
    queue.dequeue();
    queue.increamentActiveStart_Index();//this will increament active start to emulate fossile collection.
    queue.increamentActiveStart_Index();
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
TEST(UnifiedQueueTest, PositionTest){
    UnifiedQueue<Event, compareEvent> queue(10);
    int pos = queue.findInsertPosition(Event (1, 1, "a", "b", 1, true));
    EXPECT_EQ(pos, 0);
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    pos = queue.findInsertPosition(Event(3, 3, "a", "b", 1, true));
    EXPECT_EQ(pos, 1);
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    pos = queue.findInsertPosition(Event(2, 2, "a", "b", 1, true));
    EXPECT_EQ(pos, 1);
    queue.enqueue(Event(2, 2, "a", "b", 1, true)); //let this be here if u wanna add more cases
    // add rotation cases
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.enqueue(Event(5, 5, "a", "b", 1, true));
    queue.enqueue(Event(6, 6, "a", "b", 1, true));
    queue.enqueue(Event(7, 7, "a", "b", 1, true));
    
    queue.enqueue(Event(8, 8, "a", "b", 1, true));
    queue.enqueue(Event(9, 9, "a", "b", 1, true));
    queue.enqueue(Event(10, 10, "a", "b", 1, true));
    EXPECT_EQ(queue.isFull(), 1);
    queue.debug();

    queue.dequeue();
    queue.debug();
    queue.increamentActiveStart_Index() ; //these two lines replicate fossil collection and event processing
    queue.dequeue();
    queue.debug();
    queue.increamentActiveStart_Index() ;

    pos = queue.findInsertPosition(Event(11, 11, "a", "b", 1, true)); //this should be inserted at pos 0
    EXPECT_EQ(pos, 0);
    queue.enqueue(Event(11, 11, "a", "b", 1, true));
    pos = queue.findInsertPosition(Event(11, 10, "a", "b", 2, true)); //this should be inserted at pos 0
    EXPECT_EQ(pos, 0);
}

//not on zero
TEST(UnifiedQueueTest, SizeTest){
    UnifiedQueue<Event, compareEvent> queue(4);
    queue.enqueue(Event(1, 1, "a", "b", 1, true));
    queue.enqueue(Event(2, 2, "a", "b", 1, true));
    queue.enqueue(Event(3, 3, "a", "b", 1, true));
    queue.enqueue(Event(4, 4, "a", "b", 1, true));
    queue.debug();
    EXPECT_EQ(queue.size(), 4); //00-4  markers
    queue.dequeue();
    queue.increamentActiveStart_Index();
    EXPECT_EQ(queue.size(), 3); //110  markers
    queue.debug();
    queue.dequeue();
    queue.dequeue();
    queue.dequeue();
    queue.debug();
    EXPECT_EQ(queue.size(), 3); //1-40 markers
    queue.enqueue(Event(5, 5, "a", "b", 1, true)); //10-1 markers
    queue.debug();
    queue.increamentActiveStart_Index();
    queue.debug();
    queue.increamentActiveStart_Index();
    queue.increamentActiveStart_Index();
    queue.debug(); //-401 markers
    queue.dequeue();
    queue.debug();
    queue.increamentActiveStart_Index();
    queue.debug();
    EXPECT_EQ(queue.size(), 0); //0-40 markers
    EXPECT_EQ(queue.isEmpty(), true);
    queue.increamentActiveStart_Index();
    queue.debug();
    queue.dequeue();
    queue.debug();

    
}

// test for increamentActiveStart_Index

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