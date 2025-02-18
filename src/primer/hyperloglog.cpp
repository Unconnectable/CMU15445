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
  if (n_bits < 0) {
    n_bits = 0;
  }
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
  std::bitset<BITSET_CAPACITY> binary(hash);
  return binary;
}

/**
 * @brief Function that computes leading zeros.
 *
 * @param[in] bset - binary values of a given bitset
 * @returns leading zeros of given binary set
 */
/** @TODO(student) Implement this function! */

template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(student) Implement this function! */
  // 取消掉前面的nbits_位 直接从有效位开始计算
  for (int64_t i = BITSET_CAPACITY - 1 - n_bits_; i >= 0; --i) {
    if (bset[i]) {
      return static_cast<uint64_t>(BITSET_CAPACITY - n_bits_ - i);
    }
  }
  return BITSET_CAPACITY - n_bits_ + 1;
}

/**
 * @brief Adds a value into the HyperLogLog.
 *
 * @param[in] val - value that's added into hyperloglog
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(student) Implement this function! */
  hash_t hash = CalculateHash(val);
  auto binary = ComputeBinary(hash);
  // 保留n_bits 位 即 桶的编号
  uint64_t j = (binary >> (BITSET_CAPACITY - n_bits_)).to_ullong();  // 桶的编号
  uint64_t p = PositionOfLeftmostOne(binary);                        // 计算1的位置
  // 他的前面的n_bits_位是记录他的寄存器的位置 x
  std::lock_guard<std::mutex> lock(mtx_);
  // 写入操作
  registers_[j] = std::max(registers_[j], static_cast<uint8_t>(p));
}
/**
 * @brief Function that computes cardinality.
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(student) Implement this function! */
  // 读操作
  std::shared_lock<std::shared_mutex> guard(shlock_);
  double sum = 0.0;
  if (num_registers_ == 0) {
    return;
  }

  for (int32_t j = 0; j < num_registers_; ++j) {
    sum += 1.00 / std::pow(2, static_cast<double>(registers_[j]));
  }
  cardinality_ = static_cast<size_t>(std::floor(CONSTANT * num_registers_ * num_registers_ / sum));
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub
