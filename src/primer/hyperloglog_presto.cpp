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
  if (n_leading_bits < 0) {
    n_leading_bits = 0;
  }
  n_leading_bits_ = n_leading_bits;
  num_registers_ = 1 << n_leading_bits_;
  dense_bucket_.resize(num_registers_);
  cardinality_ = 0;
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  const hash_t hash_val = CalculateHash(val);
  std::bitset<64> binary(hash_val);
  size_t j = (binary >> (64 - n_leading_bits_)).to_ulong();
  int64_t tot = 0;
  // 手写一个函数朴素查找从右到左的0的数量的 lambda 函数
  auto find_first_set = [&tot, this](const std::bitset<64> &bits) {
    for (size_t i = 0; i <= bits.size() - 1 - n_leading_bits_; ++i) {
      if (!bits[i]) {
        tot++;
      } else {
        break;
      }
    }
    tot = tot != 64 ? tot : (64 - n_leading_bits_);
    return tot;
  };
  tot = find_first_set(binary);
  int64_t old_value = dense_bucket_[j].to_ullong();
  // 计算原来的值 也就是低四位加上高三位
  if (overflow_bucket_.find(j) != overflow_bucket_.end()) {
    old_value += ((overflow_bucket_[j].to_ullong()) << DENSE_BUCKET_SIZE);
  }
  int64_t new_value = std::max(old_value, tot);
  auto overflow_val = (new_value >> DENSE_BUCKET_SIZE);

  std::lock_guard<std::mutex> lock(mtx_);  // write lock

  if (overflow_val > 0) {
    overflow_bucket_[j] = overflow_val;
    new_value = new_value - (overflow_val << DENSE_BUCKET_SIZE);
    dense_bucket_[j] = new_value;
    return;
  }
  dense_bucket_[j] = new_value;

  /*
  这是一个不太好的写法 没有检查overflow_bucket_[j]是否存在 而是直接进行了调用

  old_value += ((overflow_bucket_[j].to_ullong()) << DENSE_BUCKET_SIZE);
  int64_t new_value = std::max(old_value, tot);
  overflow_bucket_[j] = overflow_val;
  new_value = new_value - (overflow_val << DENSE_BUCKET_SIZE);
  dense_bucket_[j] = new_value;

  同理 计算cardinality_的循环里面也可以不检查 是否存在
  不启用这个检查 if(overflolw_bucker_.find(j) != overflolw_bucker_.end() ) 直接调用

  for (int j = 0; j < m; ++j) {
    int64_t val = dense_bucket_[j].to_ullong();
    val += overflow_bucket_[j].to_ullong() << DENSE_BUCKET_SIZE;
    sum += 1.0 / std::pow(2, val);
  }
  */
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  std::shared_lock<std::shared_mutex> guard(shlock_);  // read lock

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
