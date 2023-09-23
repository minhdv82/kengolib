/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include "utils.h"

#include <stdint.h>

#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

namespace kl {
// we prefer composition over inheritance
// so Shape is better be a component of Tensor
class Shape final {
 public:
  typedef uint64_t s_type;
  Shape() : value_{} {}
  Shape(std::vector<s_type>& val) : value_{val} {}
  Shape(std::vector<s_type>&& val) : value_{std::move(val)} {}
  Shape(const Shape& rhs) : value_{rhs.value_} {}
  Shape(Shape& rhs) : value_{std::move(rhs.value_)} {}
  Shape(Shape&& rhs) : value_{std::move(rhs.value_)} {}
  s_type size() const {
    return std::accumulate(value_.begin(), value_.end(), 1,
                           std::multiplies<s_type>());
  }
  size_t rank() const { return (this->value_).size(); }

  bool operator==(const Shape& rhs) const noexcept {
    return this->value_ == rhs.value_;
  }

  Shape operator+(const Shape& rhs) const noexcept {
    if (this->is_none() || rhs.is_none() || !this->is_compatible(rhs))
      return Shape();
    if (this->value_ == rhs.value_) return rhs;

    const Shape& big = (this->rank() > rhs.rank()) ? *this : rhs;
    const Shape& sml = (this->rank() > rhs.rank()) ? rhs : *this;

    Shape res = big;
    auto bsz = big.rank(), ssz = sml.rank();
    for (size_t i = 0; i < bsz; ++i) {
      if (i < bsz - ssz) {
        res.value_[i] = big.value_[i];
      } else {
        res.value_[i] = std::max(big.value_[i], sml.value_[i + ssz - bsz]);
      }
    }
    return res;
  }

  bool is_none() const { return this->rank() < 1; }
  void extend(int dim) {
    int sz = this->rank();
    if (dim < -sz) return;
    if (dim < 0) dim += sz;
    this->value_.insert(value_.begin() + dim, 1);
  }

  void contract(int dim) {
    int sz = this->rank();
    if (dim >= sz || dim < -sz) return;
    if (dim < 0) dim += sz;
    s_type d = value_[dim];
    auto it = value_.erase(value_.begin() + dim);
    if (it == value_.end()) {
      *(--it) *= d;
    } else {
      (*it) *= d;
    }
  }

  bool is_compatible(const Shape& rhs) const {
    if (rhs.is_none() || this->is_none()) return true;
    auto us_it = this->value_.end(), them_it = rhs.value_.end();
    const auto b_us = this->value_.begin(), b_them = rhs.value_.begin();
    do {
      if (*(--us_it) == *(--them_it)) {
        continue;
      } else if (*us_it != 1 && *them_it != 1) {
        return false;
      }
    } while (us_it != b_us && them_it != b_them);
    return (*us_it == *them_it || *us_it == 1 || *them_it == 1);
  }

  void reshape(const std::vector<s_type>& v) {
    if (this->is_none() ||
        this->size() !=
            std::accumulate(v.begin(), v.end(), 1, std::multiplies<s_type>()))
      return;
    value_ = std::vector<s_type>(v);
  }

  void flatten() noexcept {
    if (this->is_none()) return;
    value_ = {this->size()};
  }

  auto value() const { return value_; }
  friend std::ostream& operator<<(std::ostream& o, const Shape& rhs) {
    if (rhs.is_none()) return o;
    auto it = rhs.value_.begin(), lt = rhs.value_.end();
    o << "(" << *(it++);
    while (it != lt) {
      o << ", " << *(it++);
    }
    o << ")\n";
    return o;
  }

 private:
  std::vector<s_type> value_;
};

template <typename val_type>
class Tensor {
  static Tensor None() { return Tensor(); }

 public:
  Tensor() : value_{}, shape_{} {}
  Tensor(const Shape& shape, const std::vector<val_type>& val)
      : shape_{shape}, value_{val} {}
  Tensor(Shape&& shape, std::vector<val_type>&& val)
      : value_{std::move(val)}, shape_{std::move(shape)} {}
  Tensor(const Tensor& rhs) : shape_{rhs.shape_}, value_{rhs.value_} {}
  size_t rank() const { return shape_.rank(); }
  auto size() const { return shape_.size(); }
  void reshape(const std::vector<Shape::s_type>& shape) {
    shape_.reshape(shape);
  }
  void flatten() noexcept {
    shape_.flatten();
  }  // flatten only affects shape of the tensor, not its value rep
  void extend(int dim) noexcept { shape_.extend(dim); }
  void contract(int dim) noexcept { shape_.contract(dim); }
  bool is_compatible(const Tensor& rhs) const noexcept {
    return shape_.is_compatible(rhs.shape_);
  }
  constexpr bool is_none() const { return shape_.is_none(); }

  Tensor& operator*=(val_type x) noexcept {
    if (this->is_none()) return (*this);
    for (auto& val : value_) val *= x;
    return (*this);
  }

  Tensor operator*(val_type x) const noexcept {
    if (this->is_none()) return *this;
    auto res = (*this);
    for (auto& val : res.value_) val *= x;
    return res;
  }

  Tensor operator+(val_type x) noexcept {
    if (this->is_none()) return (*this);
    auto value = this->value_;
    for (auto& val : value) val += x;
    return Tensor(this->shape_, value);
  }

  Tensor operator-(val_type x) noexcept { return (*this) + (-x); }

  Tensor operator+(const Tensor& rhs) const noexcept {
    if (this->is_none() || rhs.is_none() || !this->is_compatible(rhs))
      return Tensor::None();

    if (this->shape_ == rhs.shape_) {
      Tensor res = *this;
      for (auto i = 0; i < res.value_.size(); ++i)
        res.value_[i] += rhs.value_[i];
      return res;
    }

    const Shape shp = this->shape_ + rhs.shape_;
    std::vector<val_type> value(shp.size(), 0);
    return this->op_diff_shape(
        rhs, shp, [](val_type l, val_type r) -> val_type { return l + r; });
  }

  Tensor operator-(const Tensor& rhs) const noexcept {
    return (*this) + rhs * (-1);
  }

  Tensor operator*(const Tensor& rhs) const noexcept {
    if (this->is_none() || rhs.is_none() || !this->is_compatible(rhs))
      return Tensor::None();

    if (this->shape_ == rhs.shape_) {
      Tensor res = *this;
      for (auto i = 0; i < res.value_.size(); ++i)
        res.value_[i] *= rhs.value_[i];
      return res;
    }

    Shape shp = this->shape_ + rhs.shape_;
    return this->op_diff_shape(
        rhs, shp, [](val_type l, val_type r) -> val_type { return l * r; });
  }

  bool operator==(const Tensor& rhs) const noexcept {
    if (this->is_none() && rhs.is_none()) return true;
    return (this->shape_ == rhs.shape_ && this->value_ == rhs.value_);
  }

  auto shape() const { return shape_.value(); }

  friend std::ostream& operator<<(std::ostream& o, const Tensor& rhs) {
    o << rhs.shape_;
    return o;
  }

 private:
  Tensor op_diff_shape(const Tensor& rhs, const Shape& out_shape,
                   std::function<val_type(val_type, val_type)> bin_op) const {
    auto expand = [&out_shape](Shape& shp) {
      while (shp.rank() < out_shape.rank()) {
        shp.extend(0);
      }
    };
    auto div = [&out_shape](const Shape& shp) {
      auto res = 1;
      for (auto i = 0; i < shp.rank(); ++i) {
        if (shp.value()[i] == 1)
          res *= out_shape.value()[i];
        else
          break;
      }
      return res;
    };
    Shape l_shp = this->shape_, r_shp = rhs.shape_;
    auto const l_val = this->value_, r_val = rhs.value_;
    expand(l_shp);
    expand(r_shp);
    auto l_div = div(l_shp), r_div = div(r_shp);
    std::vector<val_type> val(out_shape.size(), 0);
    auto lsz = l_shp.size(), rsz = r_shp.size(), osz = out_shape.size();
    for (auto i = 0; i < osz; ++i) {
      auto l_idx = (i / l_div) % lsz, r_idx = (i / r_div) % rsz; // hacky but works
      val[i] = bin_op(l_val[l_idx], r_val[r_idx]);
    }
    return Tensor(out_shape, val);
  };
  Shape shape_;
  std::vector<val_type> value_;
};

}  // namespace kl