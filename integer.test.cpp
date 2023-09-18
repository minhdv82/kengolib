/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */

#include "utils.h"
#include "integer.h"

#include <gtest/gtest.h>

#include <time.h>

#include <cassert>
#include <iostream>
#include <vector>


TEST(test_dynamic, test_math) {
  Rand rng(82 + time(nullptr));
  for (int i = 0; i < 10; ++i) {
    // x and y are huge numbers
    bigint x({rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64(),
              rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64(),
              rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64()});
    bigint y({rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64(),
              rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64(),
              rng.uint64(), rng.uint64(), rng.uint64(), rng.uint64()});
    bigint e({rng.uint64()});

    bigint s = x / y, r = x % y;
    bigint xx = s * y + r;
    bigint g = gcd(x, y);
    // bigint np = pow_mod(x, e, x);
    // bigint npp = pow_mod(e, x, e + 1);
    // ASSERT_TRUE(np == 0 && npp != 0);
    ASSERT_TRUE(x % g == 0 && y % g == 0);
    ASSERT_EQ(xx, x);
    ASSERT_EQ(x * y, y * x);
    ASSERT_EQ((x + y) * x, x * (y + x));
    ASSERT_EQ(x * (x + y), x * x + x * y);
    ASSERT_EQ((x + y) - x, y);
    ASSERT_EQ(x - (x + 1), 0);
  }
};

TEST(test_static_comp, test_prime) {
  bigint e(12345), n(54321), p(56789);
  bigint enp = pow_mod(e, n, p);
  bigint x(12345678987654321);
  bigint y = make_prime(x);  // becomes 12345678987654373
  ASSERT_EQ(enp, 30482);
  ASSERT_FALSE(is_prime(x));
  ASSERT_TRUE(is_prime(y));
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}