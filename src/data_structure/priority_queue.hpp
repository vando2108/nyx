#ifndef DATA_STRUCTURE_PRIORITY_QUEUE_HPP
#define DATA_STRUCTURE_PRIORITY_QUEUE_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <list>
#include <optional>
#include <unordered_map>
#include <utility>

#include "src/common/include/define.hpp"
#include "src/utils/include/bitwise.hpp"

namespace nyx::data_structure {
template <typename T>
class priority_queue {
  typedef typename std::list<T>::iterator node_addr_t;
  typedef std::pair<uint8_t, node_addr_t> node_container_t;

  std::unordered_map<T, node_container_t> addr_map_;
  std::array<std::list<T>, common::define::size_type_bits> priorities_;

 public:
  std::size_t marker_;

 public:
  priority_queue() : marker_(0) {}
  ~priority_queue() {}

 public:
  bool is_exsit(const T&) const noexcept;
  const T& top() const noexcept;
  void push_no_update(T&&, const uint8_t&) noexcept;
  void push_and_update(T&&, const uint8_t&) noexcept;
  std::optional<T> try_pop() noexcept;
  void remove(const T&) noexcept;
  std::optional<uint8_t> get_priority(const T&) const noexcept;
  void clear() noexcept;
  size_t size() const noexcept;
};

template <typename T>
bool priority_queue<T>::is_exsit(const T& value) const noexcept {
  return addr_map_.find(value) != addr_map_.end();
}

template <typename T>
void priority_queue<T>::push_no_update(T&& value, const uint8_t& priority) noexcept {
  if (is_exsit(value)) {
    return;
  }
  push_and_update(std::forward<T>(value), priority);
}

template <typename T>
void priority_queue<T>::push_and_update(T&& value, const uint8_t& priority) noexcept {
  auto it = addr_map_.find(value);

  // Value already exsits
  if (it != addr_map_.end()) {
    node_container_t& nc = it->second;
    // Priority not changed
    if (nc.first == priority) {
      return;
    }

    priorities_[nc.first].erase(nc.second);
    if (priorities_[nc.first].empty()) {
      utils::bitwise::turn_off_bit(marker_, nc.first);
    }

    nc.first = priority;
    if (priorities_[nc.first].empty()) {
      utils::bitwise::turn_on_bit(marker_, nc.first);
    }
    priorities_[nc.first].push_back(std::forward<T>(value));
    nc.second = std::prev(priorities_[nc.first].end());

    return;
  }

  // Value not exsits
  utils::bitwise::turn_on_bit(marker_, priority);
  auto& prio_list = priorities_[priority];

  prio_list.push_back(std::forward<T>(value));
  addr_map_.emplace(prio_list.back(), std::make_pair(priority, std::prev(prio_list.end())));
}

template <typename T>
std::optional<T> priority_queue<T>::try_pop() noexcept {
  if (marker_ == 0) {
    return std::nullopt;
  }

  // const uint8_t higest_priority = utils::bitwise::lmb(marker_).value();
  const uint8_t higest_priority = 0;
  auto& prio_list = priorities_[higest_priority];
  T ret = prio_list.front();
  prio_list.pop_front();

  // addr_map_.erase(ret);

  if (prio_list.empty()) {
    utils::bitwise::turn_off_bit(marker_, higest_priority);
  }

  return ret;
}

template <typename T>
void priority_queue<T>::remove(const T& value) noexcept {
  auto it = addr_map_.find(value);
  if (it == addr_map_.end()) {
    return;
  }

  const node_container_t& nc = it->second;
  auto& prio_list = priorities_[nc.first];

  prio_list.erase(nc.second);

  if (prio_list.empty()) {
    utils::bitwise::turn_off_bit(marker_, nc.first);
  }

  addr_map_.erase(it);
}

template <typename T>
std::optional<uint8_t> priority_queue<T>::get_priority(const T& value) const noexcept {
  auto it = addr_map_.find(value);
  if (it == addr_map_.end()) {
    return std::nullopt;
  }

  return it->second.first;
}

template <typename T>
void priority_queue<T>::clear() noexcept {
  addr_map_.clear();
  for (auto& it : priorities_) {
    it.clear();
  }
  marker_ = 0;
}

template <typename T>
size_t priority_queue<T>::size() const noexcept {
  return addr_map_.size();
}
}  // namespace nyx::data_structure

#endif  // !DATA_STRUCTURE_PRIORITY_QUEUE_HPP
