/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include <stdint.h>

#include <vector>
#include <numeric>

class Shape final {
public:
  typedef uint64_t s_type;
  Shape() : rank_{-1}, value_{} { }
  Shape(const std::vector<s_type>& val) : value_{val} {
    rank_ = (val.size() > 0) ? val.size() : -1;
  };
  Shape(std::vector<s_type>&& val) : value_{std::move(val)} {};
  Shape(const Shape& rhs) : value_{rhs.value_} {}
  Shape(Shape& rhs) : value_{std::move(rhs.value_)} {}
  Shape(Shape&& rhs) : value_{std::move(rhs.value_)} {}
  s_type size() const { return std::accumulate(value_.begin(), value_.end(), 1, std::multiplies<s_type>()); }
  auto rank() const { return rank_; }
  void reshape(const std::vector<s_type>& shape);
  void flatten() noexcept;
  void extend(int dim) noexcept;
  void contract(int dim) noexcept;
  bool is_compatible(const Shape& rhs) const noexcept;
  constexpr bool is_none() const { return rank_ == -1; }

private:
  void canonize() { rank_ = (value_.size() > 0)? value_.size() : -1; }
  int rank_ = -1;
  std::vector<s_type> value_ = {};
};

template <typename T>
class Tensor {

};