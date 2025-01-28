#include <cassert>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <thread>  // NOLINT
#include <vector>

#include "common/exception.h"
#include "primer/hyperloglog.h"
#include "primer/hyperloglog_presto.h"

namespace bustub {

void HyperLogLogBasicTest() {
  auto obj = HyperLogLog<std::string>(static_cast<int16_t>(1));
  assert(obj.GetCardinality() == 0);

  obj.AddElem("Welcome to CMU DB (15-445/645)");
  obj.ComputeCardinality();

  auto ans = obj.GetCardinality();
  assert(ans == 2);
  std::cout << "Test 1 passed. Cardinality: " << ans << std::endl;

  for (uint64_t i = 0; i < 10; i++) {
    obj.AddElem("Andy");
    obj.AddElem("Connor");
    obj.AddElem("J-How");
    obj.AddElem("Kunle");
    obj.AddElem("Lan");
    obj.AddElem("Prashanth");
    obj.AddElem("William");
    obj.AddElem("Yash");
    obj.AddElem("Yuanxin");

    if (i == 0) {
      obj.ComputeCardinality();
      ans = obj.GetCardinality();
      assert(ans == 6);
      std::cout << "Test 2 passed (i == 0). Cardinality: " << ans << std::endl;
    }
  }

  obj.ComputeCardinality();
  ans = obj.GetCardinality();
  assert(ans == 6);
  std::cout << "Test 3 passed. Final Cardinality: " << ans << std::endl;
}

}  // namespace bustub

int main() {
  try {
    bustub::HyperLogLogBasicTest();
    std::cout << "All tests passed successfully!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Test failed with exception: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    std::cerr << "Test failed with an unknown error." << std::endl;
    return 1;
  }
  return 0;
}