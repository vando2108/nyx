#ifndef UTILS_BITWISE_HPP
#define UTILS_BITWISE_HPP

#include <cstddef>
#include <optional>

namespace nyx::utils::bitwise {
int clz(std::size_t x) noexcept;
int ctz(std::size_t x) noexcept;
std::optional<int> lmb(const std::size_t&) noexcept;
bool is_power_of_two(const std::size_t&) noexcept;
void turn_on_bit(std::size_t&, const uint8_t&) noexcept;
void turn_off_bit(std::size_t&, const uint8_t&) noexcept;
}  // namespace nyx::utils::bitwise

#endif  // !UTILS_BITWISE_HPP
