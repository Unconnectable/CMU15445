//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.cpp
//
// 标识: src/primer/hyperloglog.cpp
//
// 版权所有 (c) 2015-2025, 卡内基梅隆大学数据库组
//
//===----------------------------------------------------------------------===//

#include "primer/hyperloglog.h"

namespace bustub {

/** @brief 参数化构造函数。 */
template <typename KeyType>
HyperLogLog<KeyType>::HyperLogLog(int16_t n_bits) : cardinality_(0) {}

/**
 * @brief 计算二进制的函数。
 *
 * @param[in] hash - 哈希值
 * @returns 给定哈希值的二进制表示
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY> {
  /** @TODO(学生) 实现此函数！ */
  return {0};
}

/**
 * @brief 计算前导零的函数。
 *
 * @param[in] bset - 给定位集的二进制值
 * @returns 给定二进制集的前导零数量
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t {
  /** @TODO(学生) 实现此函数！ */
  return 0;
}

/**
 * @brief 将值添加到 HyperLogLog 中。
 *
 * @param[in] val - 添加到 hyperloglog 中的值
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::AddElem(KeyType val) -> void {
  /** @TODO(学生) 实现此函数！ */
}

/**
 * @brief 计算基数的函数。
 */
template <typename KeyType>
auto HyperLogLog<KeyType>::ComputeCardinality() -> void {
  /** @TODO(学生) 实现此函数！ */
}

template class HyperLogLog<int64_t>;
template class HyperLogLog<std::string>;

}  // namespace bustub