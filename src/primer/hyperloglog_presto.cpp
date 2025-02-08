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
  //防止n_leading_bits为负数
  if (n_leading_bits <= 0) n_leading_bits = 1;
  n_leading_bits_ = n_leading_bits;

  //初始化 registers_
  num_registers_ = 1 << n_leading_bits_;
  registers_.resize(num_registers_, 0);
  // 预分配内存（防止过度扩展）
  dense_bucket_.reserve(num_registers_);
  overflow_bucket_.reserve(num_registers_);

  // 初始化每个键值对
  for (int i = 0; i < num_registers_; ++i) {
    dense_bucket_[i] = std::bitset<DENSE_BUCKET_SIZE>();
    overflow_bucket_[i] = std::bitset<OVERFLOW_BUCKET_SIZE>();
  }
  cardinality_ = 0;
}

/** @brief Element is added for HLL calculation. */
template <typename KeyType>
auto HyperLogLogPresto<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  /*要add overflow bucket 和 dense bucket
  overflow bucket 是用来存储hash值的3位
  densebucker 4位
  `std::vector<std::bitset<DENSE_BUCKET_SIZE>> dense_bucket_;
  std::unordered_map<uint16_t,std::bitset<OVERFLOW_BUCKET_SIZE>> overflow_bucket_;`
  */
  auto hash = CalculateHash(val);
  auto binary = std::bitset<64>(hash);
  int tot = 0;
  for (size_t i = binary.size() - 1; i >= binary.size() - n_leading_bits_; i--) {
    //从hash的右边计算0的个数 直到遇见1
    //计算的是二进制最右侧连续的零的个数
    //它必然是一个<128的数字
    //转化位2进制的时 前三位 最高有效位 在overflow bucket中
    //剩下的4位数在dense bucket中
    if (binary[i] == 0)
      tot++;
    else
      break;
  }
  std::bitset<7> tot_bin(tot);
  //计算dense bucket的index
  std::bitset<OVERFLOW_BUCKET_SIZE> overflow_bits;

  std::lock_guard<std::mutex> guard(lock_);

  for (int i = 0; i < OVERFLOW_BUCKET_SIZE; ++i) {
    overflow_bits[i] = tot_bin[i];  // 将 tot_bin 的前 3 位填充到 overflow_bits
  }

  // 第二步：存储到 overflow_bucket_
  uint16_t overflow_index = hash & ((1 << n_leading_bits_) - 1);  // 使用哈希值计算溢出桶的索引
  overflow_bucket_[overflow_index] = overflow_bits;               // 将溢出位存入 overflow_bucket

  // 第三步：提取密集桶的低 4 位
  std::bitset<DENSE_BUCKET_SIZE> dense_bits;
  for (int i = 0; i < DENSE_BUCKET_SIZE; ++i) {
    dense_bits[i] = tot_bin[i + OVERFLOW_BUCKET_SIZE];  // 从 tot_bin 的 4~6 位填充到 dense_bits
  }

  // 第四步：存储到 dense_bucket_
  dense_bucket_.push_back(dense_bits);  // 将密集桶的值存储到 dense_bucket_ 中
}

/** @brief Function to compute cardinality. */
template <typename T>
auto HyperLogLogPresto<T>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  std::shared_lock<std::shared_mutex> guard(shlock_);
  double sum = 0.0;
  for (int i = 0; i < num_registers_; i++) {
    sum += 1.0 / (1 << registers_[i]);
  }
  if (sum == 0.0) {
    cardinality_ = 0;
    return;
  }
  double E = CONSTANT * num_registers_ * num_registers_ / sum;
  cardinality_ = static_cast<uint64_t>(E);
  return;
}

template class HyperLogLogPresto<int64_t>;
template class HyperLogLogPresto<std::string>;
}  // namespace bustub
