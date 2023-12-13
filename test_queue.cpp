// write tests using gtest to test UnifiedQUeue

#include <gtest/gtest.h>
#include "UnifiedQueue.hpp"
#include "randomEventGenerator.hpp"
#include <thread>


RandomEventGenerator r;
// test for isEmpty
TEST(UnifiedQueueTest, isEmptyTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    EXPECT_EQ(queue.isEmpty(), true);
}

// test for isFull
TEST(UnifiedQueueTest, isFullTest) {
    UnifiedQueue<Event, compareEvent> queue(10);
    EXPECT_EQ(queue.isFull(), false);
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

// test for enqueue

// test for dequeue


// test for findInsertPosition


// test for increamentActiveStart_Index

int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}