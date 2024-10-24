#ifndef DATA_STRUCTURE_UNORDERED_MAP
#define DATA_STRUCTURE_UNORDERED_MAP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <format>
#include <functional>
#include <iterator>
#include <ostream>
#include <utility>

#include "src/utils/include/bitwise.hpp"

namespace nyx::data_structure {
template <typename Key, typename Value, typename Hasher = std::hash<Key>>
class unordered_map {
 public:
  typedef Key key_type;
  typedef Value mapped_type;
  typedef std::pair<Key, Value> key_value;
  typedef Hasher hasher;

 private:
  typedef int bucket_index_t_;
  static constexpr size_t DEFAULT_BUCKET_COUNT_ = 1 << 10;
  static constexpr float MLF = 0.9;  // max load factor
  static constexpr bucket_index_t_ EMPTY_PSL_VALUE = -1;

  class iterator;

 private:
  struct bucket_t {
    key_value kv;
    bucket_index_t_ psl;

    bucket_t() : psl(EMPTY_PSL_VALUE) {}
    bucket_t(key_value _kv, bucket_index_t_ _psl) : kv(_kv), psl(_psl) {}

    friend std::ostream& operator<<(std::ostream& out, const bucket_t& bucket) {
      out << std::format("key: {}, value: {}, psl: {},", bucket.kv.first, bucket.kv.second, bucket.psl);
      return out;
    }
  };

  float lf_;
  size_t bucket_count_;
  size_t size_;
  bucket_t* buckets_;
  Hasher hasher_;

 public:
  unordered_map();
  explicit unordered_map(size_t, double lf = MLF);

  ~unordered_map() { delete[] buckets_; }

  // getter && setter
  size_t bucket_count() const noexcept { return bucket_count_; }
  size_t size() const noexcept { return size_; }
  size_t load_factor() const noexcept { return lf_; }

  // debug methods
  void buckets() const noexcept {
    // LOG(INFO) << "bucket count: " << bucket_count_;
    // for (int i = 0; i < bucket_count_; ++i) {
    //   LOG(INFO) << buckets_[i];
    // }
  }

  // public methods
  iterator insert(const Key&, Value&&) noexcept;
  iterator insert(const Key&, const Value&) noexcept;
  iterator find(const Key&) const;
  void erase(const Key&);
  bool contains(const Key&) const;
  void clear();

  // iterators
  iterator begin() { return iterator(buckets_[0]); }
  iterator end() const { return iterator(buckets_ + bucket_count_); }

  // operators
  Value& operator[](const Key&);

 private:
  // private methods
  iterator insert_internal_(bucket_t&);
  bool key_match_(const Key& key, const bucket_t& bucket) const;
  bool move_to_next_bucket_(bucket_index_t_& bucket_index) const;
  // return the bucket_index correspond to the input hash_key
  inline bucket_index_t_ kh_to_bucket_index_(const bucket_index_t_& kh) const { return kh & (bucket_count_ - 1); }
  inline bucket_index_t_ iterator_to_index_(const iterator& it) const { return it.bucket_ptr_ - buckets_; }
};

template <typename Key, typename Value, typename Hasher>
class unordered_map<Key, Value, Hasher>::iterator {
 private:
  friend class unordered_map<Key, Value, Hasher>;
  bucket_t* bucket_ptr_;

 public:
  using iterator_category = std::forward_iterator_tag;
  using pointer = key_value*;
  using reference = key_value&;

 public:
  iterator(bucket_t* ptr) : bucket_ptr_(ptr) {}

  reference operator*() const { return bucket_ptr_->kv; }
  pointer operator->() { return &(bucket_ptr_->kv); }
  bool operator==(const iterator& other) const { return bucket_ptr_ == other.bucket_ptr_; }
  bool operator!=(const iterator& other) const { return bucket_ptr_ != other.bucket_ptr_; }

  iterator& operator++() {
    ++bucket_ptr_;
    return *this;
  }
};

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::unordered_map() : bucket_count_(DEFAULT_BUCKET_COUNT_), buckets_(new bucket_t[bucket_count_]), hasher_(Hasher()) {}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::unordered_map(size_t bucket_count, double lf) : hasher_(Hasher()) {
  assert(bucket_count > 0);
  assert(lf > 0 && lf <= MLF);

  bucket_count_ = static_cast<size_t>(1) << utils::bitwise::lmb(bucket_count);
  buckets_ = new bucket_t[bucket_count_];

  lf_ = lf;
}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::iterator unordered_map<Key, Value, Hasher>::insert_internal_(bucket_t& new_bucket) {
  auto kh = hasher_(new_bucket.kv.first);
  size_t bucket_index = this->kh_to_bucket_index_(kh);

  // loop until we found an empty
  while (buckets_[bucket_index].psl != EMPTY_PSL_VALUE && bucket_index < bucket_count_) {
    if (new_bucket.psl >= buckets_[bucket_index].psl) {
      std::swap(new_bucket, buckets_[bucket_index]);
    }
    ++new_bucket.psl;
    ++bucket_index;
  }

  if (bucket_index < bucket_count_) {
    buckets_[bucket_index] = new_bucket;
    ++size_;
    return unordered_map<Key, Value, Hasher>::iterator(&buckets_[bucket_index]);
  }

  // TODO: resize the buckets
  return end();
}

template <typename Key, typename Value, typename Hasher>
bool unordered_map<Key, Value, Hasher>::unordered_map::key_match_(const Key& key, const bucket_t& bucket) const {
  if (bucket.psl != EMPTY_PSL_VALUE && hasher_(key) == hasher_(bucket.kv.first)) {
    return true;
  }
  return false;
}

template <typename Key, typename Value, typename Hasher>
bool unordered_map<Key, Value, Hasher>::unordered_map::move_to_next_bucket_(bucket_index_t_& bucket_index) const {
  if (++bucket_index >= bucket_count_) {
    return false;
  }

  if (buckets_[bucket_index].psl == EMPTY_PSL_VALUE || buckets_[bucket_index].psl <= buckets_[bucket_index - 1].psl) {
    return false;
  }

  return true;
}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::iterator unordered_map<Key, Value, Hasher>::insert(const Key& key, Value&& value) noexcept {
  bucket_t new_bucket = bucket_t(std::make_pair(key, std::forward<Value>(value)), 0);
  return insert_internal_(new_bucket);
}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::iterator unordered_map<Key, Value, Hasher>::insert(const Key& key, const Value& value) noexcept {
  bucket_t new_bucket = bucket_t(std::make_pair(key, value), 0);
  return insert_internal_(new_bucket);
}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::iterator unordered_map<Key, Value, Hasher>::find(const Key& key) const {
  auto kh = hasher_(key);
  auto bucket_index = this->kh_to_bucket_index_(kh);

  do {
    if (this->key_match_(key, buckets_[bucket_index])) {
      return unordered_map<Key, Value, Hasher>::iterator(&buckets_[bucket_index]);
    }
  } while (this->move_to_next_bucket_(bucket_index));

  return end();
}

template <typename Key, typename Value, typename Hasher>
void unordered_map<Key, Value, Hasher>::erase(const Key& key) {
  iterator to_remove_it = this->find(key);
  if (to_remove_it == end()) {
    return;
  }

  bucket_index_t_ bucket_index = iterator_to_index_(to_remove_it);
  // set bucket_to_remove as empty and move to the next bucket to start backward shifting
  buckets_[bucket_index++].psl = EMPTY_PSL_VALUE;

  for (; bucket_index < bucket_count_; ++bucket_index) {
    if (buckets_[bucket_index].psl == EMPTY_PSL_VALUE || buckets_[bucket_index].psl == 0) {
      break;
    }

    std::swap(buckets_[bucket_index - 1], buckets_[bucket_index]);
  }

  --size_;
}

template <typename Key, typename Value, typename Hasher>
bool unordered_map<Key, Value, Hasher>::contains(const Key& key) const {
  auto kh = hasher_(key);
  auto bucket_index = this->kh_to_bucket_index_(kh);

  do {
    if (this->key_match_(key, buckets_[bucket_index])) {
      return true;
    }
  } while (this->move_to_next_bucket_(bucket_index));

  return false;
}

template <typename Key, typename Value, typename Hasher>
void unordered_map<Key, Value, Hasher>::clear() {
  std::fill(buckets_, buckets_ + bucket_count_, bucket_t());
  size_ = 0;
}

template <typename Key, typename Value, typename Hasher>
Value& unordered_map<Key, Value, Hasher>::operator[](const Key& key) {
  auto it = this->find(key);
  if (it == this->end()) {
    // if this key not exist, insert an entry with default value
    it = this->insert(key, Value());
  }

  return it->second;
}

}  // namespace nyx::data_structure

#endif  // !DATA_STRUCTURE_UNORDERED_MAP
