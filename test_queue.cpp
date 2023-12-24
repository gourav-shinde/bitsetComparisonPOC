// write tests using gtest to test UnifiedQUeue

#include <gtest/gtest.h>
#include "UnifiedQueue.hpp"
#include "randomEventGenerator.hpp"
#include <thread>


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


