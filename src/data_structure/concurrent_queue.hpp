#ifndef DATA_STRUCTURE_CONCURRENT_QUEUE_HPP
#define DATA_STRUCTURE_CONCURRENT_QUEUE_HPP

#include <memory>
#include <mutex>
#include <utility>

#include "src/data_structure/circular_array.hpp"

namespace nyx {
namespace data_structure {

/**
 * @brief A thread-safe concurrent queue implementation.
 *
 * This queue is based on a CircularArray and uses a mutex for synchronization,
 * ensuring thread safety for push and pop operations.
 *
 * @tparam T The type of elements stored in the queue.
 * @tparam Alloc The allocator type used for memory management (default: std::allocator<T>).
 */
template <typename T, typename Alloc = std::allocator<T>>
class ConcurrentQueue {
 private:
  CircularArray<T, Alloc> array_;  ///< The underlying circular array for storage.
  mutable std::mutex mutex_;       ///< Mutex for ensuring thread safety.

 public:
  /**
   * @brief Constructs an empty queue with a default capacity of 1.
   *
   * @param alloc The allocator to use for memory management.
   */
  ConcurrentQueue(const Alloc& alloc = Alloc()) : array_(1, alloc) {}

  /**
   * @brief Constructs a queue with a specified initial capacity.
   *
   * @param capacity The initial capacity of the queue.
   * @param alloc The allocator to use for memory management.
   */
  explicit ConcurrentQueue(size_t capacity, const Alloc& alloc = Alloc()) : array_(capacity, alloc) {}

  /**
   * @brief Destroys the ConcurrentQueue.
   */
  ~ConcurrentQueue() {}

  /**
   * @brief Pushes an rvalue element into the queue.
   *
   * @param value The element to be added.
   */
  void push(T&& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    array_.push_back(std::forward<T>(value));
  }

  /**
   * @brief Pushes a const reference element into the queue.
   *
   * @param value The element to be added.
   */
  void push(const T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    array_.push_back(value);
  }

  /**
   * @brief Tries to pop an element from the front of the queue.
   *
   * @param value Reference to store the popped element.
   * @return True if an element was successfully popped, false if the queue was empty.
   */
  bool try_pop(T& value) {
    std::unique_lock<std::mutex> lock(mutex_);
    return array_.pop_front(value);
  }

  /**
   * @brief Gets the current size of the queue.
   *
   * @return The number of elements in the queue.
   */
  size_t size() const { return array_.size(); }

  /**
   * @brief Checks if the queue is empty.
   *
   * @return True if the queue is empty, false otherwise.
   */
  bool empty() const { return array_.empty(); }

  /**
   * @brief Checks if the queue is full.
   *
   * @return True if the queue is full, false otherwise.
   */
  bool full() const { return array_.full(); }
};

}  // namespace data_structure
}  // namespace nyx

#endif  // !DATA_STRUCTURE_CONCURRENT_QUEUE_HPP
