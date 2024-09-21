#ifndef DATA_STRUCTURE_STEALING_WORK_QUEUE_HPP
#define DATA_STRUCTURE_STEALING_WORK_QUEUE_HPP

#include <atomic>
#include <cassert>
#include <optional>
#include <utility>

#include "src/common/include/define.hpp"

namespace nyx::data_structure {
using namespace common::define;

template <typename T>
class StealingWorkQueue {
  struct CircularAtomicArray {
    size_t cap;
    std::atomic<T*> elements;

    CircularAtomicArray() = delete;
    explicit CircularAtomicArray(size_t capacity) : cap(capacity), elements{new T[capacity]} {}

    ~CircularAtomicArray() { delete[] elements; }

    size_t capacity() const noexcept { return cap; }

    void push(size_t index, T&& value) { elements[index & cap] = std::forward<T>(value); }
    T pop(size_t index) { return elements[index & cap]; }

    CircularAtomicArray* resize(size_t bot, size_t top) {
      CircularAtomicArray* ptr = new CircularAtomicArray{2 * cap};
      for (size_t i = top; i != bot; ++i) {
        ptr->push(i, pop(i));
      }
      return ptr;
    }
  };

  alignas(hardware_constructive_interference_size) std::atomic<std::size_t> top_;
  alignas(hardware_constructive_interference_size) std::atomic<std::size_t> bot_;
  std::atomic<CircularAtomicArray*> array_;

 public:
  explicit StealingWorkQueue(size_t cap = 1024);
  ~StealingWorkQueue();

  bool empty() const noexcept;
  size_t size() const noexcept;
  size_t capacity() const noexcept { return array_.load(std::memory_order_relaxed)->capacity(); }

  void push(T&&);
  std::optional<T> pop();
  std::optional<T> steal();
};

template <typename T>
StealingWorkQueue<T>::StealingWorkQueue(size_t cap) {
  top_.store(0, std::memory_order_relaxed);
  bot_.store(0, std::memory_order_relaxed);
  array_.store(new CircularAtomicArray{cap}, std::memory_order_relaxed);
}

template <typename T>
StealingWorkQueue<T>::~StealingWorkQueue() {
  delete array_.load();
}

template <typename T>
bool StealingWorkQueue<T>::empty() const noexcept {
  size_t bot = bot_.load(std::memory_order_relaxed);
  size_t top = top_.load(std::memory_order_relaxed);
  return bot <= top;
}

template <typename T>
size_t StealingWorkQueue<T>::size() const noexcept {
  size_t bot = bot_.load(std::memory_order_relaxed);
  size_t top = top_.load(std::memory_order_relaxed);
  return bot >= top ? bot - top : 0;
}

template <typename T>
void StealingWorkQueue<T>::push(T&& value) {
  size_t bot = bot_.load(std::memory_order_relaxed);
  size_t top = top_.load(std::memory_order_acquire);
  CircularAtomicArray* a = array_.load(std::memory_order_relaxed);

  if (a->capacity() < bot - top + 1) {
    // queue is full, need to resize
    auto temp = a->resize(bot, top);
    std::swap(temp, a);
    delete[] temp;
    array_.store(a, std::memory_order_relaxed);
  }

  a->push(bot, std::forward<T>(value));
  std::atomic_thread_fence(std::memory_order_release);
  bot_.store(bot + 1, std::memory_order_relaxed);
}

template <typename T>
std::optional<T> StealingWorkQueue<T>::pop() {
  size_t bot = bot_.load(std::memory_order_relaxed) - 1;
  CircularAtomicArray* a = array_.load(std::memory_order_relaxed);
  bot_.store(bot, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_seq_cst);
  size_t top = top_.load(std::memory_order_relaxed);

  std::optional<T> ret;
  if (top <= bot) {
    ret = a->pop(bot);
    if (top == bot) {
      if (!top_.compare_exchange_strong(top, top + 1, std::memory_order_seq_cst, std::memory_order_relaxed)) {
        ret = std::nullopt;
      }
      bot_.store(bot + 1, std::memory_order_relaxed);
    }
  } else {
    bot_.store(bot + 1, std::memory_order_relaxed);
  }

  return ret;
}

template <typename T>
std::optional<T> StealingWorkQueue<T>::steal() {
  size_t top = top_.load(std::memory_order_acquire);
  std::atomic_thread_fence(std::memory_order_seq_cst);
  size_t bot = bot_.load(std::memory_order_acquire);

  std::optional<T> ret;
  if (top < bot) {
    CircularAtomicArray* a = array_.load(std::memory_order_consume);
    ret = a->pop(top);
    if (!top_.compare_exchange_strong(top, top + 1, std::memory_order_seq_cst, std::memory_order_relaxed)) {
      return std::nullopt;
    }
  }

  return ret;
}
}  // namespace nyx::data_structure

#endif  // !DATA_STRUCTURE_STEALING_WORK_QUEUE_HPP
