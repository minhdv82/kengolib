/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include <random>

class Rand {
private:
  std::mt19937_64 gen_;
public:
  explicit Rand(uint64_t s) : gen_(s) {}
  int64_t int64() { return gen_(); }
  uint64_t uint64() { return gen_(); }
  uint32_t uint32(uint32_t n = 0xffffFFFF) { return static_cast<uint32_t>(gen_() % n); }
  int32_t int32() { return static_cast<int32_t>(gen_()); }
  double_t doub() { return static_cast<double_t>(gen_()) / 0xffffffffFFFFFFFF; }
  template <typename T>
  T uniform(T from, T to) {
    T gap = to - from;
    T val = static_cast<T>(gen_()) % gap;
    if (val < 0) val += gap;
    return from + val;
  }
};