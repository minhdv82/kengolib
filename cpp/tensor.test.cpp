/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#include "tensor.h"

#include <gtest/gtest.h>

#include "utils.h"

using kl::Shape;
using kl::Tensor;

Rand rng(82 + time(nullptr));

TEST(test_add, add_change_shape) {
  Shape shp1(std::vector<uint64_t>{1, 2, 3});
  Shape shp2(std::vector<uint64_t>{3, 2, 1});
  Shape shp3(std::vector<uint64_t>{   2, 3});
  Tensor<int> x1(shp1, std::vector<int>(shp1.size(), 1));
  Tensor<int> x2(shp2, std::vector<int>(shp2.size(), 2));
  Tensor<int> x3(shp2, std::vector<int>(shp3.size(), 3));
  auto z = x1 + x2;
  ASSERT_TRUE(z.is_compatible(x1));
  ASSERT_FALSE(z == x2);
  ASSERT_TRUE(z.is_compatible(z + x3));
  ASSERT_EQ(z, (z + x3) - x3);
  ASSERT_TRUE(x3.is_compatible(x3 * z));
};

TEST(test_static, static_foo) {
  Shape shp1(std::vector<uint64_t>{1, 2, 3});
  Tensor<int> x(shp1, std::vector<int>(shp1.size(), 1));

  Shape shp2(std::vector<uint64_t>{10, 3, 2, 1});
  Tensor<int> y(shp2, std::vector<int>(shp2.size(), 1));
  Tensor z = x * 5;

  EXPECT_EQ(x, x * 1);
  EXPECT_FALSE(y == y * 2);
  EXPECT_TRUE(y.is_compatible(x));
  y.flatten();
  EXPECT_EQ(y.rank(), 1);
};

TEST(test_dynamic, dynamic_foo) {
  for (int i = 0; i < 5; ++i) {
    uint64_t n1 = rng.uniform<uint64_t>(50, 100),
             n2 = rng.uniform<uint64_t>(50, 100),
             n3 = rng.uniform<uint64_t>(50, 100);
    Shape sh({n1, n2, n3}), shp({n1, n2, 1});

    std::vector<int> val1(n1 * n2 * n3, 0), val2(n1 * n2 * n3, 0), val3(n1 * n2, 0);
    for (auto& v : val1) v = rng.uniform<int32_t>(-10, 100);
    for (auto& v : val2) v = rng.uniform<int32_t>(-100, 100);
    for (auto& v : val3) v = rng.uniform<int32_t>(-100, 100);
    Tensor<int> t(sh, val1), u(sh, val2), v(shp, val3);
    ASSERT_EQ(t + v, v + t);
    ASSERT_EQ(t * v, v * t);
    ASSERT_NO_THROW(t.extend(-1));
    ASSERT_NO_THROW(t.contract(2));
  }
};

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}