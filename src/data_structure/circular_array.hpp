#ifndef DATA_STRUCTURE_CIRCULAR_ARRAY
#define DATA_STRUCTURE_CIRCULAR_ARRAY

#include <memory>

#include "src/utils/include/bitwise.hpp"

/**
 * @brief A circular array data structure with dynamic resizing.
 *
 * This class provides a circular array implementation with power-of-two capacity,
 * ensuring efficient modulo operations. The capacity grows dynamically as needed.
 *
 * @tparam T The type of elements stored in the array.
 * @tparam Alloc The allocator type used for memory management (default: std::allocator<T>).
 */
template <typename T, typename Alloc = std::allocator<T>>
class CircularArray {
 private:
  T* data_;          ///< Pointer to the allocated memory.
  size_t capacity_;  ///< Current capacity of the array.
  Alloc alloc_;      ///< Allocator for memory management.
  size_t front_, rear_;

#define alloc_traits std::allocator_traits<decltype(alloc_)>;

 public:
  CircularArray() = delete;

  /**
   * @brief Constructs a CircularArray with the given initial capacity.
   *
   * The capacity is adjusted to the next power of two for efficient indexing.
   *
   * @param initial_capacity The initial capacity of the array.
   * @param alloc The allocator to use for memory management.
   */
  explicit CircularArray(size_t initial_capacity, const Alloc& alloc = Alloc()) : capacity_(initial_capacity), alloc_(alloc) {
    if (capacity_ == 0) {
      capacity_ = 1;
    } else {
      capacity_ = (1 << nyx::utils::bitwise::lmb(capacity_));
    }

    data_ = alloc_.allocate(capacity_);
  }

  /**
   * @brief Destroys the CircularArray and deallocates memory.
   */
  ~CircularArray() {
    clear();
    alloc_.deallocate(data_, capacity_);
  }

  bool push_back(T&& value, bool grow_if_full = true) {
    if (full()) {
      if (!grow_if_full) {
        return false;
      }
      grow();
    }

    new (&data_[index(rear_)]) T(std::forward<T>(value));
    ++rear_;

    return true;
  }

  bool push_back(const T& value, bool grow_if_full = true) {
    if (full()) {
      if (!grow_if_full) {
        return false;
      }
      grow();
    }

    new (&data_[index(rear_)]) T(value);
    ++rear_;

    return true;
  }

  bool pop_front(T& value) {
    if (empty()) return false;

    value = std::move(data_[index(front_)]);
    data_[index(front_)].~T();

    ++front_;

    return true;
  }

  bool pop_front() {
    if (empty()) return false;

    data_[index(front_)].~T();
    ++front_;

    return true;
  }

  /**
   * @brief Accesses an element by index.
   *
   * The index is masked to ensure circular behavior.
   *
   * @param index The index of the element to access.
   * @return Reference to the element at the given index.
   */
  T& operator[](size_t i) { return data_[index(i)]; }

  /**
   * @brief Returns the current capacity of the array.
   *
   * @return The capacity of the array.
   */
  size_t capacity() const { return capacity_; }

  size_t size() const { return rear_ - front_; }

  bool full() const { return size() == capacity_; }

  bool empty() const { return (front_ == rear_); }

  /**
   * @brief Clears all elements in the array.
   */
  void clear() { while (pop_front()); }

 private:
  /**
   * @brief Resizes the array to a new capacity.
   *
   * The new capacity is double the current capacity. Existing elements are moved,
   * and the old memory is deallocated.
   */
  void grow() {
    size_t new_capacity = capacity_ << 1;
    T* new_data = alloc_.allocate(new_capacity);

    for (size_t i = 0; i < capacity_; ++i) {
      new (&new_data[i]) T(std::move(data_[i]));
      data_[i].~T();
    }

    alloc_.deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_capacity;
  }

  size_t index(size_t i) const { return (i & (capacity_ - 1)); }
};

#endif  // DATA_STRUCTURE_CIRCULAR_ARRAY
