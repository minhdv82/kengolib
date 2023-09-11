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
    bigint y({rng.int64(), rng.int64(), rng.int64(), rng.int64()});
    bigint e({rng.int64()});

    bigint s = x / y, r = x % y;
    bigint xx = s * y + r;
    bigint g = gcd(x, y);
    bigint np = pow_mod(x, e, x);
    bigint npp = pow_mod(e, x, e + 1);
    assert(np == 0 && npp != 0);
    assert(x % g == 0 && y % g == 0);
    assert(xx == x);
    assert(x * y == y * x);
    assert((x + y) * x == x * (y + x));
    assert(x * (x + y) == x * x + x * y);
    assert((x + y) - x == y);
    assert(x - (x + 1) == 0);
    // euclid(x, y);
  }

  bigint e({12345}), n({54321}), p({56789});
  bigint enp = pow_mod(e, n, p);
  assert(enp == 30482);

  std::cout << "Test passed\n";

  return 0;
}