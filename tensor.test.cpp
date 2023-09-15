/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */

#include "tensor.h"

#include <iostream>
#include <cassert>
#include <vector>

auto main() -> int {
  kl::Shape shp1(std::vector<uint64_t>{1, 2, 3});
  kl::Tensor<int> x(shp1, std::vector<int>(1, 6));
  
  kl::Shape shp2(std::vector<uint64_t>{10, 3, 2, 1});
  kl::Tensor<int> y(shp2, std::vector<int>(1, 6));

  assert(x.is_compatible(y));

  std::cout << "Test passed!\n" ;

  return 0;
}