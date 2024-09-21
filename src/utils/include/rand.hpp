#ifndef UTILS_RAND_HPP
#define UTILS_RAND_HPP

#include <vector>

namespace nyx::utils::rand {
class Xorshift {
 public:
  Xorshift() {}

  static std::size_t next() {
    auto temp = x_ ^ (x_ << 11);

    x_ = y_, y_ = z_, z_ = w_;

    return w_ = w_ ^ (w_ >> 19) ^ temp ^ (temp >> 8);
  }

 private:
  static std::size_t x_, y_, z_, w_;
};

std::vector<std::size_t> rand_list(size_t size, size_t element_size);
}  // namespace nyx::utils::rand

#endif  // !UTILS_RAND_HPP
