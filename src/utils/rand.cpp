#include "include/rand.hpp"

#include <vector>

#include "src/utils/include/time.hpp"

namespace nyx::utils::rand {
std::size_t Xorshift::x_ = 210820016061997;
std::size_t Xorshift::y_ = time::now();
std::size_t Xorshift::z_ = time::now();
std::size_t Xorshift::w_ = time::now();

std::vector<std::size_t> rand_list(std::size_t size, std::size_t element_size) {
  std::vector<std::size_t> ret(size);
  std::for_each(ret.begin(), ret.end(), [&](std::size_t &x) { x = Xorshift::next() % element_size; });

  return ret;
}
}  // namespace nyx::utils::rand
