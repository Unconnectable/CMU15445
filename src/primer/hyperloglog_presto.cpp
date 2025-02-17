//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.cpp
//
// Identification: src/primer/hyperloglog_presto.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog_presto.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLogPresto<KeyType>::HyperLogLogPresto(int16_t n_leading_bits) {
  if (n_leading_bits >= 0) {
    n_leading_bits_ = n_leading_bits;
    num_registers_ = 1 << n_leading_bits_;
    dense_bucket_.resize(num_registers_);
    cardinality_ = 0;
  }
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  const hash_t hash_val = CalculateHash(val);
  std::bitset<64> binary(hash_val);
  size_t j = (binary >> (64 - n_leading_bits_)).to_ulong();
  int64_t tot = 0;
  /*
  for (size_t i = 0; i <= binary.size() - 1 - n_leading_bits_; i++) {
    if (!binary[i]) {
      tot++;
    } else {
      break;
    }
  }
*/
  std::bitset<64> remian_binary(hash_val - (j << (64 - n_leading_bits_)));
  tot = remian_binary._Find_first();
  tot = tot != 64 ? tot : (64 - n_leading_bits_);
  int64_t old_value = dense_bucket_[j].to_ullong();
  if (overflow_bucket_.find(j) != overflow_bucket_.end()) {
    old_value += ((overflow_bucket_[j].to_ullong()) << DENSE_BUCKET_SIZE);
  }
  int64_t new_value = std::max(old_value, tot);
  auto overflow_val = (new_value >> DENSE_BUCKET_SIZE);
  if (overflow_val > 0) {
    overflow_bucket_[j] = overflow_val;
    new_value = new_value - (overflow_val << DENSE_BUCKET_SIZE);
    dense_bucket_[j] = new_value;
    return;
  }
  dense_bucket_[j] = new_value;
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  std::shared_lock<std::shared_mutex> guard(shlock_);

  double sum = 0.0;
  int m = dense_bucket_.size();
  if (m == 0) {
    return;
  }
  for (int j = 0; j < m; ++j) {
    int64_t val = dense_bucket_[j].to_ullong();
    if (overflow_bucket_.find(j) != overflow_bucket_.end()) {
      val += overflow_bucket_[j].to_ullong() << DENSE_BUCKET_SIZE;
    }
    sum += 1.0 / std::pow(2, val);
  }
  cardinality_ = static_cast<size_t>(std::floor(CONSTANT * m * m / sum));
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub
