/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */

#include "integer.h"
#include "utils.h"

#include "time.h"

#include <iostream>
#include <vector>
#include <cassert>

int main() {
  Rand rng(82 + time(nullptr));
  for (int i = 0; i < 100; ++i) {
    bigint x({rng.int64(), rng.int64(), rng.int64(), rng.int32()});
    bigint y({rng.int64(), rng.int64(), rng.int64()});
    bigint xy = x * y;
    bigint yx = y * x;
    xy.display();
    yx.display();
    
    assert(xy == yx);
    // assert(xy == (xy / r) * r + (xy % r));
  }

  assert(bigint({2, 4, 6}) * bigint({3}) == bigint({2*3, 4*3, 6*3}));


  std::cout << "Test passed\n";

  return 0;
}