#include "include/bitwise.hpp"

namespace nyx::utils::bitwise {
int clz(size_t x) noexcept {
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

int ctz(size_t x) noexcept {
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

std::optional<int> lmb(size_t x) noexcept {
  if (x == 0) {
    return std::nullopt;
  }

  return 64 - clz(x);
}

bool is_power_of_two(size_t x) noexcept { return (x & (x - 1)) == 0; }
}  // namespace nyx::utils::bitwise
