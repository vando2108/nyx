#include <gtest/gtest.h>

#include "src/data_structure/priority_queue.hpp"

namespace nyx::data_structure {

class PriorityQueueTest : public ::testing::Test {
 protected:
  priority_queue<int> pq;

  void SetUp() override {}

  void TearDown() override { pq.clear(); }
};

TEST_F(PriorityQueueTest, IsExistReturnsFalseForNonExistentElement) {
  int element = 10;
  EXPECT_FALSE(pq.is_exsit(element));
}

TEST_F(PriorityQueueTest, PushNoUpdateAddsElementWithCorrectPriority) {
  int element = 5;
  uint8_t priority = 3;

  pq.push_no_update(std::move(element), priority);
  EXPECT_TRUE(pq.is_exsit(5)) << "Pushed element not exits in pq";
  EXPECT_NE(pq.marker_, 0) << "Marker is eq 0";
  EXPECT_EQ(pq.get_priority(5), priority) << pq.get_priority(5).has_value();
}

TEST_F(PriorityQueueTest, PushAndUpdateUpdatesPriorityOfExistingElement) {
  int element = 8;
  uint8_t initial_priority = 2;
  uint8_t updated_priority = 5;

  pq.push_no_update(std::move(element), initial_priority);
  pq.push_and_update(std::move(element), updated_priority);

  EXPECT_TRUE(pq.is_exsit(8));
  EXPECT_EQ(pq.get_priority(8).value(), updated_priority);
}

TEST_F(PriorityQueueTest, TryPopReturnsElementWithHighestPriority) {
  int element1 = 1, element2 = 2;
  pq.push_no_update(std::move(element1), 1);
  pq.push_no_update(std::move(element2), 3);

  int popped;
  ASSERT_TRUE(pq.try_pop(popped));
  EXPECT_EQ(popped, 2);  // Element with higher priority should pop first
}

TEST_F(PriorityQueueTest, RemoveDeletesElementCorrectly) {
  int element = 7;
  pq.push_no_update(std::move(element), 2);

  pq.remove(element);
  EXPECT_FALSE(pq.is_exsit(element));
}

TEST_F(PriorityQueueTest, GetPriorityReturnsNoneForNonExistentElement) {
  int element = 3;
  auto priority = pq.get_priority(element);
  EXPECT_FALSE(priority.has_value());
}

// Test pushing and popping a single element
TEST_F(PriorityQueueTest, SingleElementPushPop) {
  int element = 15;
  pq.push_no_update(std::move(element), 5);

  int popped;
  ASSERT_TRUE(pq.try_pop(popped));
  EXPECT_EQ(popped, 15);

  // Ensure the queue is empty after pop
  EXPECT_FALSE(pq.try_pop(popped));
}

// Test pushing elements with the same priority
TEST_F(PriorityQueueTest, SamePriorityElements) {
  int element1 = 10, element2 = 20;
  pq.push_no_update(std::move(element1), 4);
  pq.push_no_update(std::move(element2), 4);

  int popped1;
  ASSERT_TRUE(pq.try_pop(popped1));
  EXPECT_TRUE(popped1 == 10);  // Any order is allowed

  int popped2;
  ASSERT_TRUE(pq.try_pop(popped2));
  EXPECT_NE(popped1, popped2);  // Both should be different elements

  // Ensure the queue is empty
  EXPECT_FALSE(pq.try_pop(popped1));
}

// Test popping from an empty queue
TEST_F(PriorityQueueTest, PopFromEmptyQueueReturnsNone) {
  int popped;
  EXPECT_FALSE(pq.try_pop(popped));
}

// Test removing an element that does not exist
TEST_F(PriorityQueueTest, RemoveNonExistentElement) {
  int element = 100;
  EXPECT_NO_THROW(pq.remove(element));  // Removing non-existent element should not throw
  EXPECT_FALSE(pq.is_exsit(element));
}

// Test priority boundary values
TEST_F(PriorityQueueTest, PriorityBoundaryValues) {
  int element1 = 1, element2 = 2;
  pq.push_no_update(std::move(element1), 0);   // Minimum priority
  pq.push_no_update(std::move(element2), 63);  // Maximum priority

  int popped;
  ASSERT_TRUE(pq.try_pop(popped));
  EXPECT_EQ(popped, 2);  // Max priority element should pop first

  ASSERT_TRUE(pq.try_pop(popped));
  EXPECT_EQ(popped, 1);  // Then min priority element
}

// Test updating priority for a non-existent element
TEST_F(PriorityQueueTest, UpdatePriorityForNonExistentElement) {
  int element = 9;
  uint8_t new_priority = 7;

  pq.push_and_update(std::move(element), new_priority);  // Element does not exist, so it should be added
  EXPECT_TRUE(pq.is_exsit(9));
  EXPECT_EQ(pq.get_priority(9), new_priority);
}

// Test adding and removing multiple elements with different priorities
TEST_F(PriorityQueueTest, AddRemoveMultipleElements) {
  int element1 = 3, element2 = 6, element3 = 9;
  pq.push_no_update(std::move(element1), 2);
  pq.push_no_update(std::move(element2), 4);
  pq.push_no_update(std::move(element3), 1);

  // Remove the element with highest priority (element2)
  pq.remove(element2);
  EXPECT_FALSE(pq.is_exsit(6));

  // Pop the remaining elements and check if they're in the correct order
  int popped1;
  ASSERT_TRUE(pq.try_pop(popped1));
  EXPECT_EQ(popped1, 3);

  int popped2;
  ASSERT_TRUE(pq.try_pop(popped2));
  EXPECT_EQ(popped2, 9);

  // Queue should be empty
  EXPECT_FALSE(pq.try_pop(popped1));
}

// Test duplicate elements handling
TEST_F(PriorityQueueTest, DuplicateElements) {
  int element = 5;
  pq.push_no_update(std::move(element), 2);

  // Push the same element with a different priority
  pq.push_and_update(5, 4);

  // Check if priority was updated
  EXPECT_EQ(pq.get_priority(5), 4);

  // Pop it and check if it's the correct element
  int popped;
  ASSERT_TRUE(pq.try_pop(popped));
  EXPECT_EQ(popped, 5);

  // Queue should be empty after popping the only element
  EXPECT_FALSE(pq.try_pop(popped));
}

}  // namespace nyx::data_structure
