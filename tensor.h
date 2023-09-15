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
static Shape None;
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
  void reshape(std::vector<s_type> shape);
  void extend(int dim);
  void contract(int dim);
  bool is_compatible(const Shape& rhs) const;
  bool is_none() const { return rank_ == -1; }

private:
  int rank_;
  std::vector<s_type> value_ = {};
};

Shape Shape::None = Shape();

template <typename T>
class Tensor {

};