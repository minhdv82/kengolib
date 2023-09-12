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
    bigint x({rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64()});
    bigint y({rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64()});
    bigint e({rng.uint64()});

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
  }

  bigint e(12345), n(54321), p(56789);
  bigint enp = pow_mod(e, n, p);
  assert(enp == 30482);

  bigint x(12345678987654321);
  assert(!is_prime(x));
  x = make_prime(x); // becomes 12345678987654373
  assert(is_prime(x));

  std::cout << "Test passed\n";

  return 0;
}