#ifndef DATA_STRUCTURE_UNORDERED_MAP
#define DATA_STRUCTURE_UNORDERED_MAP

#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
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

  class iterator;

 private:
  typedef size_t bucket_index_t_;

  struct bucket_t {
    key_value kv;
    bucket_index_t_ psl;

    bucket_t() : psl(-1) {}
    bucket_t(key_value _kv, bucket_index_t_ _psl) : kv(_kv), psl(_psl) {}
  };

  const size_t DEFAULT_CAP = 1'000;
  const float MLF = 0.9;  // max load factor

  float lf_;
  size_t cap_;
  size_t size_;
  bucket_t* buckets_;
  Hasher hasher_;

 public:
  unordered_map();
  explicit unordered_map(size_t, double lf = 0.9);

  ~unordered_map() { delete[] buckets_; }

  // getter && setter
  size_t capacity() const noexcept { return cap_; }
  size_t size() const noexcept { return size_; }
  size_t load_factor() const noexcept { return lf_; }

  // public methods
  void insert(const Key&, Value&&);
  iterator find(const Key&) const;
  void remove(const Key&);

 private:
  // private methods

  bool key_match_(const Key& key, const bucket_t& bucket) const {
    if (bucket.psl != -1 && Hasher(key) == Hasher(bucket.key)) {
      return true;
    }
    return false;
  }

  bool move_to_next_bucket_(bucket_index_t_& bucket_index) const {
    if (++bucket_index >= cap_) {
      return false;
    }

    if (buckets_[bucket_index].psl == -1 || buckets_[bucket_index].psl <= buckets_[bucket_index - 1]) {
      return false;
    }

    return true;
  }

  // return the bucket_index correspond to the input hash_key
  size_t kh_to_bucket_index_(const size_t& kh) const { return kh & (cap_ - 1); }
};

template <typename Key, typename Value, typename Hasher>
class unordered_map<Key, Value, Hasher>::iterator {
 private:
  bucket_t* bucket_ptr_;

 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::pair<const Key, Value>;
  using pointer = value_type*;
  using reference = value_type&;

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
unordered_map<Key, Value, Hasher>::unordered_map() : cap_(DEFAULT_CAP), buckets_(new bucket_t[cap_]), hasher_(Hasher()) {}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::unordered_map(size_t cap, double lf) : hasher_(Hasher()) {
  assert(cap > 0);
  assert(lf > 0 && lf <= MLF);

  cap_ = static_cast<size_t>(1) << utils::bitwise::lmb(cap);
  buckets_ = new bucket_t[cap_];

  lf_ = lf;
}

template <typename Key, typename Value, typename Hasher>
void unordered_map<Key, Value, Hasher>::insert(const Key& key, Value&& value) {
  auto kh = hasher_(key);
  size_t bucket_index = this->kh_to_bucket_index_(kh);
  bucket_t bucket = buckets_[bucket_index];
  bucket_t new_bucket = bucket_t(std::make_pair(key, value), 0);

  // loop until we found an empty
  while (bucket.psl != -1) {
    if (new_bucket.psl == bucket.psl) {
      std::swap(new_bucket, bucket);
    }
    ++new_bucket.psl;
    ++bucket_index;
  }

  buckets_[bucket_index] = new_bucket;
}

template <typename Key, typename Value, typename Hasher>
unordered_map<Key, Value, Hasher>::iterator unordered_map<Key, Value, Hasher>::find(const Key& key) const {
  auto kh = hasher_(key);
  size_t bucket_index = this->kh_to_bucket_index_(kh);

  do {
    if (this->key_match_(key, bucket_index)) {
      return unordered_map<Key, Value, Hasher>::iterator(&buckets_[bucket_index]);
    }
  } while (this->move_to_next_bucket_(bucket_index));
}

template <typename Key, typename Value, typename Hasher>
void unordered_map<Key, Value, Hasher>::remove(const Key& key) {}

}  // namespace nyx::data_structure

#endif  // !DATA_STRUCTURE_UNORDERED_MAP
