#include "include/rand.hpp"

#include "src/utils/include/time.hpp"

namespace nyx::utils::rand {
std::size_t Xorshift::x_ = 210820016061997;
std::size_t Xorshift::y_ = time::now();
std::size_t Xorshift::z_ = time::now();
std::size_t Xorshift::w_ = time::now();

std::vector<size_t> rand_list(size_t size, size_t element_size) noexcept {
  std::vector<size_t> ret(size);
  std::for_each(ret.begin(), ret.end(), [&](size_t &x) { x = Xorshift::next() % element_size; });

  return ret;
}

std::vector<std::pair<size_t, size_t>> rand_list_pair(size_t size, size_t first_limit, size_t second_limit) noexcept {
  std::vector<std::pair<size_t, size_t>> ret(size);
  std::for_each(ret.begin(), ret.end(), [&](auto &x) { x = {Xorshift::next() % first_limit, Xorshift::next() % second_limit}; });

  return ret;
}
}  // namespace nyx::utils::rand
