#include "integer.h"

#include <iostream>
#include <vector>

int main() {
  bigint y = bigint(std::vector<uint64_t>{32, 15});
  bigint z = bigint(std::vector<uint64_t>{77, 32, 11});
  // bigint x = y;
  bigint x = y + z;

  // std::cout << (x > 8) << std::endl;
  // std::cout << (x == 8) << std::endl;

  std::cout << x.val_[2] << std::endl;

  return 0;
}