#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "src/data_structure/concurrent_queue.hpp"

using namespace nyx::data_structure;

// Test fixture for ConcurrentQueue
class ConcurrentQueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Initialize a queue with a capacity of 16 for testing
    queue_ = std::make_unique<ConcurrentQueue<int>>(16);
  }

  void TearDown() override { queue_.reset(); }

  std::unique_ptr<ConcurrentQueue<int>> queue_;
};

// Test push and size
TEST_F(ConcurrentQueueTest, PushAndSize) {
  EXPECT_EQ(queue_->size(), 0);  // Initially empty

  for (int i = 0; i < 16; ++i) {
    queue_->push(i);
    EXPECT_EQ(queue_->size(), i + 1);  // Size after each push
  }

  EXPECT_TRUE(queue_->full());  // Queue should be full after 16 pushes
}

// Test try_pop
TEST_F(ConcurrentQueueTest, TryPop) {
  for (int i = 0; i < 16; ++i) {
    queue_->push(i);
  }

  int value = 0;
  for (int i = 0; i < 16; ++i) {
    EXPECT_TRUE(queue_->try_pop(value));  // Pop each element
    EXPECT_EQ(value, i);                  // Verify the popped value
  }

  EXPECT_FALSE(queue_->try_pop(value));  // Queue is now empty
}

// Test empty
TEST_F(ConcurrentQueueTest, Empty) {
  EXPECT_TRUE(queue_->empty());  // Initially empty

  queue_->push(1);
  EXPECT_FALSE(queue_->empty());  // Not empty after push

  int value = 0;
  queue_->try_pop(value);
  EXPECT_TRUE(queue_->empty());  // Empty after pop
}

// Test full
TEST_F(ConcurrentQueueTest, Full) {
  EXPECT_FALSE(queue_->full());  // Not full initially

  for (int i = 0; i < 16; ++i) {
    queue_->push(i);
  }
  EXPECT_TRUE(queue_->full());  // Full after reaching capacity

  int value = 0;
  queue_->try_pop(value);
  EXPECT_FALSE(queue_->full());  // Not full after popping
}

// Test thread safety
TEST_F(ConcurrentQueueTest, ThreadSafety) {
  const int num_threads = 4;
  const int num_elements_per_thread = 4;  // Each thread pushes 4 elements

  // Lambda function for pushing elements
  auto push_func = [this](int start, int end) {
    for (int i = start; i < end; ++i) {
      queue_->push(i);
    }
  };

  // Lambda function for popping elements
  auto pop_func = [this](int expected_count) {
    int count = 0;
    int value = 0;
    while (count < expected_count) {
      if (queue_->try_pop(value)) {
        ++count;
      }
    }
  };

  // Create threads for pushing
  std::vector<std::thread> push_threads;
  for (int i = 0; i < num_threads; ++i) {
    push_threads.emplace_back(push_func, i * num_elements_per_thread, (i + 1) * num_elements_per_thread);
  }

  // Create threads for popping
  std::vector<std::thread> pop_threads;
  for (int i = 0; i < num_threads; ++i) {
    pop_threads.emplace_back(pop_func, num_elements_per_thread);
  }

  // Wait for all threads to finish
  for (auto& thread : push_threads) {
    thread.join();
  }
  for (auto& thread : pop_threads) {
    thread.join();
  }

  // Verify the queue is empty after all operations
  EXPECT_TRUE(queue_->empty());
  EXPECT_EQ(queue_->size(), 0);
}
