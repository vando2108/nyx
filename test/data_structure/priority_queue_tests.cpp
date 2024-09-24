#include <gtest/gtest.h>

#include <array>
#include <list>
#include <optional>
#include <unordered_map>

#include "src/data_structure/priority_queue.hpp"

namespace nyx::data_structure {

class PriorityQueueTest : public ::testing::Test {
 protected:
  priority_queue<int> pq;

  void SetUp() override {
    // Optionally initialize the priority queue here
  }

  void TearDown() override {
    // Optionally clear up resources after each test
  }
};

TEST_F(PriorityQueueTest, IsExistReturnsFalseForNonExistentElement) {
  int element = 10;
  EXPECT_FALSE(pq.is_exsit(element));
}

TEST_F(PriorityQueueTest, PushNoUpdateAddsElementWithCorrectPriority) {
  int element = 5;
  uint8_t priority = 3;

  pq.push_no_update(std::move(element), priority);
  EXPECT_TRUE(pq.is_exsit(5));
  EXPECT_EQ(pq.get_priority(5), priority);
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

  auto popped = pq.try_pop();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 2);  // Element with higher priority should pop first
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

  auto popped = pq.try_pop();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 15);

  // Ensure the queue is empty after pop
  EXPECT_FALSE(pq.try_pop().has_value());
}

// Test pushing elements with the same priority
TEST_F(PriorityQueueTest, SamePriorityElements) {
  int element1 = 10, element2 = 20;
  pq.push_no_update(std::move(element1), 4);
  pq.push_no_update(std::move(element2), 4);

  auto popped1 = pq.try_pop();
  ASSERT_TRUE(popped1.has_value());
  EXPECT_TRUE(popped1.value() == 10 || popped1.value() == 20);  // Any order is allowed

  auto popped2 = pq.try_pop();
  ASSERT_TRUE(popped2.has_value());
  EXPECT_NE(popped1.value(), popped2.value());  // Both should be different elements

  // Ensure the queue is empty
  EXPECT_FALSE(pq.try_pop().has_value());
}

// Test popping from an empty queue
TEST_F(PriorityQueueTest, PopFromEmptyQueueReturnsNone) {
  auto popped = pq.try_pop();
  EXPECT_FALSE(popped.has_value());
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

  auto popped = pq.try_pop();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 2);  // Max priority element should pop first

  popped = pq.try_pop();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 1);  // Then min priority element
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
  auto popped1 = pq.try_pop();
  ASSERT_TRUE(popped1.has_value());
  EXPECT_EQ(popped1.value(), 3);

  auto popped2 = pq.try_pop();
  ASSERT_TRUE(popped2.has_value());
  EXPECT_EQ(popped2.value(), 9);

  // Queue should be empty
  EXPECT_FALSE(pq.try_pop().has_value());
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
  auto popped = pq.try_pop();
  ASSERT_TRUE(popped.has_value());
  EXPECT_EQ(popped.value(), 5);
  //
  // // Queue should be empty after popping the only element
  // EXPECT_FALSE(pq.try_pop().has_value());
}

}  // namespace nyx::data_structure
