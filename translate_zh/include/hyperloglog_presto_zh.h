//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog_presto.h
//
// Identification: src/include/primer/hyperloglog_presto.h
//
// Copyright (c) 2015-2025, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <bitset>
#include <memory>
#include <mutex>  // NOLINT
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "common/util/hash_util.h"

/** @brief 稠密桶的大小。 */
#define DENSE_BUCKET_SIZE 4
/** @brief 溢出桶的大小。 */
#define OVERFLOW_BUCKET_SIZE 3

/** @brief 总桶大小。 */
#define TOTAL_BUCKET_SIZE (DENSE_BUCKET_SIZE + OVERFLOW_BUCKET_SIZE)

namespace bustub {

template <typename KeyType>
class HyperLogLogPresto {
  /**
   * 注意：测试框架将会使用 GetDenseBucket 和 GetOverflow 函数，
   * 因此这些函数不能被删除。必须使用 dense_bucket_ 数据结构。
   */

  /** @brief HLL 的常量。 */
  static constexpr double CONSTANT = 0.79402;

 public:
  /** @brief 禁用默认构造函数。 */
  HyperLogLogPresto() = delete;

  explicit HyperLogLogPresto(int16_t n_leading_bits);

  /** @brief 返回 dense_bucket_ 数据结构。 */
  auto GetDenseBucket() const -> std::vector<std::bitset<DENSE_BUCKET_SIZE>> { return dense_bucket_; }

  /** @brief 返回指定索引的溢出桶。 */
  auto GetOverflowBucketofIndex(uint16_t idx) { return overflow_bucket_[idx]; }

  /** @brief 返回集合的基数。 */
  auto GetCardinality() const -> uint64_t { return cardinality_; }

  auto AddElem(KeyType val) -> void;

  auto ComputeCardinality() -> void;

 private:
  /** @brief 计算哈希值。
   *
   * @param[in] val
   *
   * @returns 哈希值
   */
  inline auto CalculateHash(KeyType val) -> hash_t {
    Value val_obj;
    if constexpr (std::is_same<KeyType, std::string>::value) {
      val_obj = Value(VARCHAR, val);
      return bustub::HashUtil::HashValue(&val_obj);
    }
    if constexpr (std::is_same<KeyType, int64_t>::value) {
      return static_cast<hash_t>(val);
    }
    return 0;
  }

  /** @brief 保存稠密桶（也称为寄存器）的数据结构。 */
  std::vector<std::bitset<DENSE_BUCKET_SIZE>> dense_bucket_;

  /** @brief 保存溢出桶的数据结构。 */
  std::unordered_map<uint16_t, std::bitset<OVERFLOW_BUCKET_SIZE>> overflow_bucket_;

  /** @brief 存储基数值。 */
  uint64_t cardinality_;

  // TODO(student) - 可根据需要添加更多数据结构
};

}  // namespace bustub
