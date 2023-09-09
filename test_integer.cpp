#include "integer.h"

#include <iostream>
#include <vector>

int main() {
  bigint y = bigint(std::vector<uint64_t>{32, 88, 96});
  bigint z = bigint(std::vector<uint64_t>{77, 11});
  // bigint x = y;
  bigint x = z * y;

  // std::cout << (x > 8) << std::endl;
  // std::cout << (x == 8) << std::endl;

  x.display();

  // bigint foo(std::vector<uint64_t>{0, 16});
  // foo = foo >> 2;
  // std::cout << foo.size() << std::endl;
  // std::cout << foo.val_[0] << std::endl;

  return 0;
}