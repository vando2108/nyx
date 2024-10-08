#include "include/bitwise.hpp"

namespace nyx::utils::bitwise {
int clz(std::size_t x) noexcept {
  // clang-format off
  size_t y;
  int n = 64;
  y = x >> 32; if (y != 0) {n -= 32; x = y;}
  y = x >> 16; if (y != 0) {n -= 16; x = y;}
  y = x >> 8;  if (y != 0) {n -= 8;  x = y;}
  y = x >> 4;  if (y != 0) {n -= 4;  x = y;}
  y = x >> 2;  if (y != 0) {n -= 2;  x = y;}
  y = x >> 1;  if (y != 0) { return n - 2; }
  // clang-format on

  return n - x;
}

unsigned clz(std::uint8_t x) {
  static constexpr std::uint8_t clz_lookup[16] = {4, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
  auto upper = x >> 4;
  auto lower = x & 0x0F;
  return upper ? clz_lookup[upper] : 4 + clz_lookup[lower];
}

int ctz(std::size_t x) noexcept {
  if (x == 0) {
    return 64;
  }

  int n = 64;
  for (int i = 32; i >= 1; i /= 2) {
    size_t y = x << i;
    if (y != 0) {
      n -= i;
      x = y;
    }
  }

  return n - 1;
}

int lmb(const std::size_t& x) noexcept {
  if (x == 0) {
    return -1;
  }

  return 64 - clz(x) - 1;
}

bool is_power_of_two(const std::size_t& x) noexcept { return x > 0 && (x & (x - 1)) == 0; }

void turn_on_bit(std::size_t& x, const uint8_t& pos) noexcept { x |= 1LL << pos; }

void turn_off_bit(std::size_t& x, const uint8_t& pos) noexcept { x &= ~(1LL << pos); }
}  // namespace nyx::utils::bitwise
