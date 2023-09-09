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
    bigint x({rng.int64(), rng.int64(), rng.int64(), rng.int64()});
    bigint y({rng.int64(), rng.int64(), rng.int64(), rng.int64(), rng.int64()});
    uint64_t q = rng.int64();
    bigint s = y / q, r = y % q;

    assert(x * y == y * x);
    assert(x + y == y + x);
    assert(r < q);
    assert(y == (s * q) + r);
  }
  std::cout << "Test passed\n";

  return 0;
}