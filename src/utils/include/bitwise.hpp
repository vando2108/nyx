#ifndef UTILS_BITWISE_HPP
#define UTILS_BITWISE_HPP

#include <cstddef>
#include <optional>

namespace nyx::utils::bitwise {
int clz(size_t x) noexcept;
int ctz(size_t x) noexcept;
std::optional<int> lmb(size_t) noexcept;
bool is_power_of_two(size_t) noexcept;
}  // namespace nyx::utils::bitwise

#endif  // !UTILS_BITWISE_HPP
