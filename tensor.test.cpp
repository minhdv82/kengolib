/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */

#include "tensor.h"

#include <gtest/gtest.h>

using kl::Tensor;
using kl::Shape;

Shape shp1(std::vector<uint64_t>{1, 2, 3});
Tensor<int> x(shp1, std::vector<int>(1, 6));

Shape shp2(std::vector<uint64_t>{10, 3, 2, 1});
Tensor<int> y(shp2, std::vector<int>(1, 6));
Tensor z = x * 5;

TEST(Foo, Bar) {
  EXPECT_EQ(x, x * 1);
  EXPECT_FALSE(y == y * 2);
  EXPECT_TRUE(y.is_compatible(x));
}

int main(int argc, char** argv) { 
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}