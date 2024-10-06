#ifndef nyx_COMMON_DEFINE_HPP
#define nyx_COMMON_DEFINE_HPP

#include <climits>
#include <cstddef>
#include <cstdint>

namespace nyx::common::define {
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 │ L1_CACHE_BYTES │ L1_CACHE_SHIFT │ __cacheline_aligned │ ...
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

constexpr std::size_t kPaddingSize = hardware_constructive_interference_size - sizeof(std::size_t);

constexpr std::uint8_t sizeof_size_t = sizeof(std::size_t) * CHAR_BIT;
}  // namespace nyx::common::define

#endif  // !nyx_COMMON_DEFINE_HPP
