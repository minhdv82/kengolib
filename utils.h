/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

// #include <stdint.h>

#include <algorithm>
#include <random>

class Rand {
private:
  std::mt19937_64 gen_;
public:
  explicit Rand(uint64_t s) : gen_(s) {}
  uint64_t int64() { return gen_(); }
  uint32_t int32() { return static_cast<uint32_t>(gen_()); }
  double_t doub() { return static_cast<double_t>(gen_()) / 0xffffffffFFFFFFFF; }
};