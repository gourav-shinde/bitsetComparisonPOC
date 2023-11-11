#include <gtest/gtest.h>
#include"combinedqueue.hpp"

TEST(QueueTest, EnqueueAndDequeue) {
  CombinedQueue queue(3);
  Event event1(1, 1, "a", "b", 1, true);
  Event event2(2, 2, "a", "b", 1, true);
  Event event3(3, 3, "a", "b", 1, true);

  queue.enqueue(event1);
  queue.enqueue(event2);
  queue.enqueue(event3);

  // EXPECT_EQ(queue.dequeue(), event1);
  // EXPECT_EQ(queue.dequeue(), event2);
  // EXPECT_EQ(queue.dequeue(), event3);
}

TEST(QueueTest, FindInsertPosition) {
  CombinedQueue queue(3);
  Event event1(1, 1, "a", "b", 1, true);
  Event event2(2, 2, "a", "b", 1, true);
  Event event3(3, 3, "a", "b", 1, true);

  queue.enqueue(event1);
  queue.enqueue(event2);

  EXPECT_EQ(queue.findInsertPosition(event3), 2);
}

TEST(QueueTest, DequeueEmptyQueue) {
  CombinedQueue queue(3);

  EXPECT_THROW(queue.dequeue(), std::out_of_range);
}