#ifndef DATA_STRUCTURE_GROWING_CIRCULAR_ARRAY
#define DATA_STRUCTURE_GROWING_CIRCULAR_ARRAY

// re-implemented from the xenium for learning purpose.

#include <atomic>
#include <cstddef>

#include "src/utils/include/bitwise.hpp"

namespace nyx::data_structure {
template <typename T, std::size_t MinCapacity = 64, std::size_t MaxCapacity = 1LL << 31>
struct LockfreeGrowingCircularArray {
  // Validate arguments
  static_assert(nyx::utils::bitwise::is_power_of_two(MinCapacity), "MinCapacity must be a power of two");
  static_assert(nyx::utils::bitwise::is_power_of_two(MaxCapacity), "MaxCapacity must be a power of two");
  static_assert(MinCapacity <= MaxCapacity, "MinCapacity must be less than or equal MaxCapacity");

 private:
  std::size_t size_;
  std::atomic<std::size_t> capacity_;
  std::atomic<T*>* ring_;

 public:
  // Constructor & Destructor
  LockfreeGrowingCircularArray();
  ~LockfreeGrowingCircularArray();

  // Public methods
  std::size_t capacity() { return capacity_.load(std::memory_order_relaxed); }
};
}  // namespace nyx::data_structure

#endif  // !DATA_STRUCTURE_GROWING_CIRCULAR_ARRAY
