//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// Identification: src/primer/hyperloglog.cpp
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"

namespace bustub {

/** @brief Parameterized constructor. */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) {
  cardinality_ = 0;
  if (n_bits <= 0) n_bits = 1;  // 设置一个合理的最小值
  n_bits_ = n_bits;
  num_registers_ = (1 << n_bits);
  registers_.resize(num_registers_, 0);
}

/**
 * @brief Function that computes binary.
 *
 * @param[in] hash
 * @returns binary of a given hash
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(student) Implement this function! */
  return std::bitset<BITSET_CAPACITY>(hash);

  // return {0};
}

/**
 * @brief Function that computes leading zeros.
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
/** @TODO(student) Implement this function! */
/*
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {

  for (int32_t i = n_bits_; i < BITSET_CAPACITY; ++i) {
    if (bset[i] == 1) return static_cast<uint64_t>(i + 1 - n_bits_);
  }
  return static_cast<uint64_t>(0);
}
*/
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** 遍历剩余比特集以找到最左边的1 */
  for (int32_t i = 0; i < BITSET_CAPACITY - n_bits_; ++i) {
    if (bset[i + n_bits_] == 1) {
      return static_cast<uint64_t>(i + 1);
    }
  }
  return static_cast<uint64_t>(BITSET_CAPACITY - n_bits_);
}

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  auto hash = CalculateHash(val);
  auto binary = ComputeBinary(hash);
  std::bitset<BITSET_CAPACITY> binary_shifed = binary >> (BITSET_CAPACITY - n_bits_);
  uint64_t j = binary_shifed.to_ullong();
  uint64_t p = PositionOfLeftmostOne(binary);
  //他的前面的n_bits_位是记录他的寄存器的位置 x
  //从n_bits_开始 until 找到1 是记录新的位置 也就是p
  // regis[j] = max(res[j],p)
  //BUSTUB_ASSERT(j >= 0 && int32_t(j) < num_registers_, "invalid bucket position");

  std::lock_guard<std::mutex> guard(lock_);
  registers_[j] = std::max(registers_[j], static_cast<uint8_t>(p));
}
/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  double sum = 0.0;
  for (int32_t j = 0; j < num_registers_; ++j) {
    sum += 1.0 / pow(2, -registers_[j]);
  }
  if (sum == 0.0) {
    cardinality_ = 0.0;
    return;
  }
  double E = CONSTANT * num_registers_ * num_registers_ / sum;
  cardinality_ = E;
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
