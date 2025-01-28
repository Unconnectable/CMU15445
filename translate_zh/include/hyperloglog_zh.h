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
  /*
  GetCardinality()：返回当前估计的基数。
  AddElem(KeyType val)：向 HyperLogLog 中添加一个元素。
  ComputeCardinality()：计算并更新基数的估计值。
  CalculateHash(KeyType val)：计算元素的哈希值。
  ComputeBinary(const hash_t &hash)：将哈希值转换为二进制形式。
  PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset)：计算二进制位集中最左侧 1 的位置。
  */

  explicit HyperLogLog(int16_t n_bits){
    n_bits_ = n_bits;
    num_registers_ = 1 << n_bits_;
    registers_ = std::vector<uint8_t>(num_registers_, 0); 
  }

  /**
   * @brief 获取基数值。
   *
   * @returns 基数值
   */
  auto GetCardinality() { return cardinality_; }

  auto AddElem(KeyType val) -> void{
    hash_t hash = CalculateHash(val);
    std::bitset<BITSET_CAPACITY> binary = ComputeBinary(hash);
    uint64_t j = hash >> (BITSET_CAPACITY - n_bits_);//保留前面的n_bits位 使用>>达成
    uint32_t p = PositionOfLeftmostOne(binary);
    registers_[j] = std::max(registers_[j], p);
  }
  auto ComputeCardinality() -> void{
  
  }

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

  auto ComputeBinary(const hash_t &hash) const -> std::bitset<BITSET_CAPACITY>{
    //哈希转换为二进制 return bitset
   std::bitset<BITSET_CAPACITY> binary(hash);
   return binary;
  }

  auto PositionOfLeftmostOne(const std::bitset<BITSET_CAPACITY> &bset) const -> uint64_t{
    //计算二进制位集中最左侧 1 的位置
    for (size_t i = n_bits; i < BITSET_CAPACITY; i++) {
      if (bset[i] == 1) {
        return i + 1 - n_bits;
      }
    }
    return BITSET_CAPACITY-n_bits;
  }

  /** @brief 基数值。 */
  size_t cardinality_;

  /** @todo (学生) 可以添加支持 HyperLogLog 的数据结构 */
  int16_t n_bits_;
  /** @brief 前导位数 */
  int32_t num_registers_;
  /** @brief 寄存器数量=2^b */
  std::vector<uint8_t> registers_;
};

}  // namespace bustub