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
  Shape() : value_{{}} {}
  Shape(std::vector<s_type> val) : value_{val} {}
  Shape(std::vector<s_type>&& val) : value_{std::move(val)} {}
  Shape(const Shape& rhs) : value_{rhs.value_} {}
  Shape(Shape& rhs) : value_{std::move(rhs.value_)} {}
  Shape(Shape&& rhs) : value_{std::move(rhs.value_)} {}
  s_type size() const { return std::accumulate(value_.begin(), value_.end(), 1, std::multiplies<s_type>()); }
  auto rank() const { return value_.size(); }
  void reshape(const std::vector<s_type>&);
  void flatten() noexcept;
  void extend(int) noexcept;
  void contract(int) noexcept;
  bool is_compatible(const Shape&) const noexcept;
  constexpr bool is_none() const { return this->rank() < 1; }

private:
  std::vector<s_type> value_;
};

template <typename val_type>
class Tensor {
static Tensor None() { return Tensor(); }
public:
  Tensor() : value_{}, shape_{Shape()} {}
  Tensor(const Shape& shape, const std::vector<val_type>& val) : value_{val}, shape_{shape} {}
  Tensor(Shape&& shape, std::vector<val_type>&& val) : value_{std::move(val)}, shape_{std::move(shape)} {}
  auto rank() const { return shape_.size(); }
  void reshape(const std::vector<s_type>& shape) { shape_.reshape(shape); }
  void flatten() noexcept { shape_.flatten(); }
  void extend(int dim) noexcept { shape_.extend(dim); }
  void contract(int dim) noexcept { shape_.contract(dim); }
  bool is_compatible(const Tensor& rhs) const noexcept { return shape_.is_compatible(rhs.shape_); }
  constexpr bool is_none() const { shape_.is_none(); }

  bool operator > (const Tensor& rhs) const noexcept {
    if (!this->is_compatible(rhs)) return false;
    return this->rank() > rhs.rank();
  }

  Tensor& operator *= (val_type x) noexcept {
    if (this->is_none()) return (*this);
    for (auto& val : value_) val *= x;
    return (*this)
  } // sclar product

  Tensor operator += (val_type x) noexcept {
    if (this->is_none()) return (*this);
    for (auto& val : value_) val += x;
    return (*this);
  } // sclar addition

  Tensor operator -= (val_type x) noexcept {
    x = -x;
    (*this) += x;
    return (*this);
  }

  Tensor operator + (const Tensor& rhs) const noexcept {
    if (!this->is_compatible(rhs)) return Tensor::None();
    Tensor& res = (this > rhs)? this : rhs;

    return res;
  }
  Tensor operator * (const Tensor& rhs) const noexcept; // hadamard product

private:
  Shape shape_;
  std::vector<val_type> value_;
};

} // namespace kl