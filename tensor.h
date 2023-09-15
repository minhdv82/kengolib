/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include "types.h"

#include <stdint.h>

#include <vector>
#include <numeric>

namespace kl {
class Shape final {
public:
  typedef uint64_t s_type;
  Shape() : rank_{-1}, value_{} { }
  Shape(const vector<s_type>& val) : value_{val} {
    rank_ = (val.size() > 0) ? val.size() : -1;
  };
  Shape(vector<s_type>&& val) : value_{std::move(val)} {};
  Shape(const Shape& rhs) : value_{rhs.value_} {}
  Shape(Shape& rhs) : value_{std::move(rhs.value_)} {}
  Shape(Shape&& rhs) : value_{std::move(rhs.value_)} {}
  s_type size() const { return std::accumulate(value_.begin(), value_.end(), 1, std::multiplies<s_type>()); }
  auto rank() const { return rank_; }
  void reshape(const vector<s_type>& shape);
  void flatten() noexcept;
  void extend(int dim) noexcept;
  void contract(int dim) noexcept;
  bool is_compatible(const Shape& rhs) const noexcept;
  constexpr bool is_none() const { return rank_ == -1; }

private:
  void canonize() { rank_ = (value_.size() > 0)? value_.size() : -1; }
  int rank_ = -1;
  vector<s_type> value_ = {};
};

template <typename val_type>
class Tensor {
static Tensor None() { return Tensor(); }
public:
  Tensor() : value_{}, shape_{Shape()} {}
  Tensor(const Shape& shape, const vector<val_type>& val) : value_{val}, shape_{shape} {}
  Tensor(Shape&& shape, vector<val_type>&& val) : value_{std::move(val)}, shape_{std::move(shape)} {}
  auto rank() const { return shape_.rank_; }
  void reshape(const vector<s_type>& shape) { shape_.reshape(shape); }
  void flatten() noexcept { shape_.flatten(); }
  void extend(int dim) noexcept { shape_.extend(dim); }
  void contract(int dim) noexcept { shape_.contract(dim); }
  bool is_compatible(const Tensor& rhs) const noexcept { return shape_.is_compatible(rhs.shape_); }
  constexpr bool is_none() const { shape_.is_none(); }

  Tensor operator * (val_type x) const noexcept {
    if (this->is_none()) return (*this);
    vector<val_type> val = x * value_;
    return Tensor(this->shape_, val);
  } // sclar product

  Tensor operator * (val_type x) const noexcept {
    if (this->is_none()) return (*this);
    vector<val_type> val = value_ + x;
    return Tensor(this->shape_, val);
  } // sclar addition

  Tensor operator + (const Tensor& rhs) const noexcept;
  Tensor operator - (const Tensor& rhs) const noexcept;
  Tensor operator * (const Tensor& rhs) const noexcept; // hadamard product

private:
  Shape shape_;
  vector<val_type> value_;
};

} // namespace kl