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
    bigint y({rng.int64(), rng.int64(), rng.int64()});
    uint64_t q = rng.int64();

    bigint s = y / q;
    bigint r = y % q;
    bigint yy = s * q + r;
    std::cout << q << std::endl;
    s.display();
    r.display();
    y.display();
    yy.display();
    assert(x * y == y * x);
    assert((x + y) * x == x * (y + x));
    assert(x * (x + y) == x * x + x * y);
    assert((x + y) - x == y);
    assert(r + s == s + r);
    assert(r < q);
    assert(yy <= y);
    assert(y == yy);
  }
  std::cout << "Test passed\n";

  return 0;
}