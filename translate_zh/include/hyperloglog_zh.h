//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// hyperloglog.h
//
// 标识: src/include/primer/hyperloglog.h
//
// 版权所有 (c) 2015-2025, 卡内基梅隆大学数据库组
//
//===----------------------------------------------------------------------===//

#pragma once

#include <bitset>
#include <memory>
#include <mutex>  // NOLINT
#include <string>
#include <utility>
#include <vector>

#include "common/util/hash_util.h"

/** @brief 位集流的容量。 */
#define BITSET_CAPACITY 64

namespace bustub {

template <typename KeyType>
class HyperLogLog {
  /** @brief HLL 的常量。 */
  static constexpr double CONSTANT = 0.79402;

 public:
  /** @brief 禁用默认构造函数。 */
  HyperLogLog() = delete;

  explicit HyperLogLog(int16_t n_bits);

  /**
   * @brief 获取基数值。
   *
   * @returns 基数值
   */
  auto GetCardinality() { return cardinality_; }

  auto AddElem(KeyType val) -> void;

  auto ComputeCardinality() -> void;

 private:
  /**
   * @brief 计算给定值的哈希值。
   *
   * @param[in] val - 值
   * @returns 给定输入值的哈希整数
   */
  inline auto CalculateHash(KeyType val) -> hash_t {
    Value val_obj;
    if constexpr (std::is_same<KeyType, std::string>::value) {
      val_obj = Value(VARCHAR, val);
    } else {
      val_obj = Value(BIGINT, val);
    }
    return bustub::HashUtil::HashValue(&val_obj);
  }

  auto ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY>;

  auto PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t;

  /** @brief 基数值。 */
  size_t cardinality_;

  /** @todo (学生) 可以添加支持 HyperLogLog 的数据结构 */
};

}  // namespace bustub