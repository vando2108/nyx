#include "include/time.hpp"

#include <chrono>

namespace nyx::utils::time {
std::size_t now() {
  const auto p1 = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
}
}  // namespace nyx::utils::time
