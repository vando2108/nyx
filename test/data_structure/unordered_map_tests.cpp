#include <gtest/gtest.h>

#include "src/data_structure/unordered_map.hpp"

namespace nyx::data_structure {

class UnorderedMapTest : public ::testing::Test {
 protected:
  unordered_map<int, std::string> map;

  void SetUp() override {}

  void TearDown() override {
    map.clear();  // Clean up after each test case
  }
};

// Test inserting a single element and checking if it exists
TEST_F(UnorderedMapTest, InsertElement) {
  int key = 42;
  std::string value = "hello";

  map.insert(key, value);
  EXPECT_TRUE(map.contains(key)) << "Key should exist after insertion";
  EXPECT_EQ(map[key], value) << "Inserted value should match";
}

// Test looking up a non-existent element
TEST_F(UnorderedMapTest, FindNonExistentElement) {
  int key = 100;
  map.insert(key, "abc");
  EXPECT_EQ(map.find(key + 1), map.end()) << "Non-existent key should return false";
}

// Test inserting multiple elements and verifying their existence
TEST_F(UnorderedMapTest, InsertMultipleElements) {
  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");

  EXPECT_TRUE(map.contains(1)) << "Key 1 should exist";
  EXPECT_TRUE(map.contains(2)) << "Key 2 should exist";
  EXPECT_TRUE(map.contains(3)) << "Key 3 should exist";

  EXPECT_EQ(map[1], "one");
  EXPECT_EQ(map[2], "two");
  EXPECT_EQ(map[3], "three");
}

// Test removing an element and checking if it was removed
TEST_F(UnorderedMapTest, RemoveElement) {
  int key = 10;
  std::string value = "test";

  map.insert(key, std::move(value));
  map.erase(key);

  EXPECT_FALSE(map.contains(key)) << "Key should not exist after removal";
}

// Test inserting and updating an element
TEST_F(UnorderedMapTest, InsertAndUpdateElement) {
  int key = 20;
  std::string initial_value = "initial";
  std::string updated_value = "updated";

  map.insert(key, initial_value);
  EXPECT_EQ(map[key], initial_value) << "Initial value should be inserted";

  map.insert(key, updated_value);  // Should update the existing key
  EXPECT_EQ(map[key], updated_value) << "Value should be updated";
}

// Test inserting with max load factor and rehashing
TEST_F(UnorderedMapTest, InsertWithMaxLoadFactor) {
  for (int i = 0; i < 100; ++i) {
    map.insert(i, "value" + std::to_string(i));
  }

  // Ensure all elements are still accessible
  for (int i = 0; i < 100; ++i) {
    EXPECT_TRUE(map.contains(i)) << "Key " << i << " should exist after rehashing";
  }

  // Verify correct values
  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(map[i], "value" + std::to_string(i)) << "Value for key " << i << " should match";
  }
}

// Test handling collision (if applicable in your open addressing)
TEST_F(UnorderedMapTest, HandleCollisions) {
  int key1 = 15;
  int key2 = 31;  // Intentionally causing a hash collision with key1

  map.insert(key1, "value1");
  map.insert(key2, "value2");

  EXPECT_EQ(map[key1], "value1") << "Key1 should map to value1";
  EXPECT_EQ(map[key2], "value2") << "Key2 should map to value2";

  map.erase(key1);
  EXPECT_FALSE(map.contains(key1)) << "Key1 should be removed";
  EXPECT_TRUE(map.contains(key2)) << "Key2 should still exist";
}

// Test getting size after insertions and removals
TEST_F(UnorderedMapTest, SizeAfterOperations) {
  EXPECT_EQ(map.size(), 0) << "Initial size should be 0";

  map.insert(1, "one");
  map.insert(2, "two");
  EXPECT_EQ(map.size(), 2) << "Size should be 2 after two insertions";

  map.erase(1);
  EXPECT_EQ(map.size(), 1) << "Size should be 1 after removal";
}

// Test inserting duplicate keys (updating existing values)
TEST_F(UnorderedMapTest, InsertDuplicateKeys) {
  int key = 99;
  std::string value1 = "first";
  std::string value2 = "second";

  map.insert(key, value1);
  EXPECT_EQ(map[key], value1) << "First value should be inserted";

  map.insert(key, value2);  // Update existing key
  EXPECT_EQ(map[key], value2) << "Value should be updated with the second insertion";
}

}  // namespace nyx::data_structure
