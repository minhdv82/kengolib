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
  std::vector<val_type> : value_{} {};
  vector(val_type x) : value_{x} {}
  vector(const vector& rhs) : value_{rhs.value_} {}
  vector(vector& rhs) : value_{rhs.value_} {}
  vector(vector&& rhs) : value_{std::move(rhs.value_)} {}
  size_t size() const { return value_.size(); }
  val_type sum() const { return std::accumulate(value_.begin(), value_.end(), 0); }
  val_type mul() const { return std::accumulate(value_.begin(), value_.end(), 1, std::multiplies<val_type>()); }
  vector& operator += (val_type x) {
    for (auto& val : value_) val += x;
    return (*this);
  }
  vector& operator -= (val_type x) {
    return (*this) += (-x);
  }
  vector& operator *= (val_type x) {
    for (auto& val : value_) val *= x;
    return this;
  }
  auto begin() const noexcept { return value_.begin(); }
  auto end() const noexcept { return value_.end(); }
  auto insert(auto it, auto val) noexcept { return value_.insert(it, val); }
  auto operator [] (auto idx) const { return value_[idx]; }
  auto erase(auto it) { return value_.erase(it); }
};

} // namespace kl