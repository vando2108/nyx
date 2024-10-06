#ifndef DATA_STRUCTURE_PRIORITY_QUEUE_HPP
#define DATA_STRUCTURE_PRIORITY_QUEUE_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
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
 public:
  typedef typename std::list<T>::iterator node_addr_t;
  typedef std::pair<uint8_t, node_addr_t> node_container_t;
  typedef typename std::unordered_map<T, node_container_t> addr_map_t;
  typedef typename addr_map_t::iterator addr_map_it_t;

 private:
  const node_container_t empty_node_ = std::make_pair(-1, node_addr_t());
  const addr_map_it_t empty_addr_map_it = addr_map_it_t();

  addr_map_t addr_map_;
  std::array<std::list<T>, common::define::sizeof_size_t> priorities_;

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
  bool try_pop(T&) noexcept;
  void remove(const T&) noexcept;
  std::optional<uint8_t> get_priority(const T&) const noexcept;
  void clear() noexcept;
  size_t size() const noexcept;

 private:
  void push_and_update_internal_(T&&, const uint8_t&, addr_map_it_t&) noexcept;
};

template <typename T>
bool priority_queue<T>::is_exsit(const T& value) const noexcept {
  auto it = addr_map_.find(value);
  return it != addr_map_.end() && it->second != empty_node_;
}

template <typename T>
void priority_queue<T>::push_no_update(T&& value, const uint8_t& priority) noexcept {
  assert(priority < common::define::sizeof_size_t);
  auto it = addr_map_.find(value);
  if (it != addr_map_.end() && it->second != empty_node_) {
    return;
  }
  push_and_update_internal_(std::forward<T>(value), priority, it);
}

template <typename T>
void priority_queue<T>::push_and_update(T&& value, const uint8_t& priority) noexcept {
  assert(priority < common::define::sizeof_size_t);
  auto it = addr_map_it_t();
  push_and_update_internal_(std::forward<T>(value), priority, it);
}

template <typename T>
void priority_queue<T>::push_and_update_internal_(T&& value, const uint8_t& priority, addr_map_it_t& it) noexcept {
  if (it == addr_map_it_t()) {
    it = addr_map_.find(value);
  }

  // Value already exsits
  if (it != addr_map_.end() && it->second != empty_node_) {
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
  addr_map_[prio_list.back()] = std::make_pair(priority, std::prev(prio_list.end()));
}

template <typename T>
bool priority_queue<T>::try_pop(T& out) noexcept {
  if (marker_ == 0) {
    return false;
  }

  const int8_t higest_priority = 0;
  // const int8_t higest_priority = utils::bitwise::lmb(marker_);
  auto& prio_list = priorities_[higest_priority];
  out = prio_list.front();
  prio_list.pop_front();

  // addr_map_[out] = empty_node_;

  if (prio_list.empty()) {
    utils::bitwise::turn_off_bit(marker_, higest_priority);
  }

  return true;
}

template <typename T>
void priority_queue<T>::remove(const T& value) noexcept {
  auto it = addr_map_.find(value);
  if (it == addr_map_.end() || it->second == empty_node_) {
    return;
  }

  const node_container_t& nc = it->second;
  auto& prio_list = priorities_[nc.first];

  prio_list.erase(nc.second);

  if (prio_list.empty()) {
    utils::bitwise::turn_off_bit(marker_, nc.first);
  }

  addr_map_[it->first] = empty_node_;
}

template <typename T>
std::optional<uint8_t> priority_queue<T>::get_priority(const T& value) const noexcept {
  auto it = addr_map_.find(value);
  if (it == addr_map_.end() || it->second == empty_node_) {
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
