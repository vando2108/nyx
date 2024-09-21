#include <gtest/gtest.h>

#include "src/data_structure/stealing_work_queue.hpp"

using namespace nyx;

class StealingWorkQueueTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Initialize any resources needed before each test
  }

  void TearDown() override {
    // Clean up any resources needed after each test
  }

  // Create a StealingWorkQueue instance for testing
  nyx::data_structure::StealingWorkQueue<int> queue;
};

TEST_F(StealingWorkQueueTest, InitializationTest) {
  EXPECT_TRUE(queue.empty());
  EXPECT_EQ(queue.size(), 0);
}

TEST_F(StealingWorkQueueTest, PushAndPopTest) {
  queue.push(1);
  queue.push(2);
  queue.push(3);

  EXPECT_EQ(queue.size(), 3);

  auto value = queue.pop();
  ASSERT_TRUE(value.has_value());
  EXPECT_EQ(value.value(), 3);

  auto value2 = queue.pop();
  ASSERT_TRUE(value2.has_value());
  EXPECT_EQ(value2.value(), 2);
  //
  // value = queue.pop();
  // ASSERT_TRUE(value.has_value());
  // EXPECT_EQ(value.value(), 3);
  //
  // EXPECT_TRUE(queue.empty());
}

// TEST_F(StealingWorkQueueTest, StealTest) {
//   queue.push(1);
//   queue.push(2);
//   queue.push(3);
//
//   std::optional<int> stolen_value;
//   std::thread thief([&stolen_value, ] { stolen_value = queue.steal(); });
//
//   thief.join();
//
//   ASSERT_TRUE(stolen_value.has_value());
//   EXPECT_EQ(stolen_value.value(), 1);  // Steal from the front of the queue
//
//   EXPECT_EQ(queue.size(), 2);
//
//   auto value = queue.pop();
//   ASSERT_TRUE(value.has_value());
//   EXPECT_EQ(value.value(), 3);
//
//   value = queue.pop();
//   ASSERT_TRUE(value.has_value());
//   EXPECT_EQ(value.value(), 2);
//
//   EXPECT_TRUE(queue.empty());
// }
//
// TEST_F(StealingWorkQueueTest, ConcurrentPushAndPop) {
//   const int num_elements = 1000;
//   std::thread producer([&queue, num_elements] {
//     for (int i = 0; i < num_elements; ++i) {
//       queue.push(i);
//     }
//   });
//
//   std::thread consumer([&queue, num_elements] {
//     for (int i = 0; i < num_elements; ++i) {
//       while (queue.pop() == std::nullopt) {
//         std::this_thread::yield();
//       }
//     }
//   });
//
//   producer.join();
//   consumer.join();
//
//   EXPECT_TRUE(queue.empty());
// }
//
// TEST_F(StealingWorkQueueTest, ResizeTest) {
//   const int initial_capacity = queue.capacity();
//   for (int i = 0; i < initial_capacity; ++i) {
//     queue.push(i);
//   }
//
//   queue.push(initial_capacity);  // This should trigger a resize
//
//   EXPECT_GT(queue.capacity(), initial_capacity);
//   EXPECT_EQ(queue.size(), initial_capacity + 1);
//
//   for (int i = initial_capacity; i >= 0; --i) {
//     auto value = queue.pop();
//     ASSERT_TRUE(value.has_value());
//     EXPECT_EQ(value.value(), i);
//   }
//
//   EXPECT_TRUE(queue.empty());
// }
