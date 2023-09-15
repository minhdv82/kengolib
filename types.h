/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include <vector>
#include <numeric>

namespace kl {
template <typename val_type>
struct vector {
  std::vector<val_type> value_;
  vector();
  size_t size() const { return value_.size(); }
  val_type sum() const { return std::accumulate(value_.begin(), value_.end(), 0); }
  val_type mul() const { return std::accumulate(value_.begin(), value_.end(), 1, std::multiplies<val_type>()); }
  vector& operator += (val_type x) {
    for (auto& val : value_) val += x;
    return this;
  }
  vector& operator -= (val_type x) {
    for (auto& val : value_) val -= x;
    return this;
  }
  vector& operator *= (val_type x) {
    for (auto& val : value_) val *= x;
    return this;
  }
  auto begin() const noexcept { return value_.begin(); }
  auto end() const noexcept { return value_.end(); }
};

} // namespace kl