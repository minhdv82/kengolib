/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include <stdint.h>

#include <vector>
#include <numeric>

namespace kl {
class Shape final {
public:
  typedef uint64_t s_type;
  Shape() : value_{{}} {}
  Shape(std::vector<s_type>& val) : value_{val} {}
  Shape(std::vector<s_type>&& val) : value_{std::move(val)} {}
  Shape(const Shape& rhs) : value_{rhs.value_} {}
  Shape(Shape& rhs) : value_{std::move(rhs.value_)} {}
  Shape(Shape&& rhs) : value_{std::move(rhs.value_)} {}
  s_type size() const { return std::accumulate(value_.begin(), value_.end(), 1, std::multiplies<s_type>()); }
  size_t rank() const { return (this->value_).size(); }

  bool operator == (const Shape& rhs) const noexcept {
    return this->value_ == rhs.value_;
  }

  bool is_none() const { return this->rank() < 1; }
  void extend(int dim) {
    int sz = this->size();
    if (dim < -sz) return;
    if (dim < 0) dim += sz;
    this->value_.insert(value_.begin() + dim, 1);
  }

  void contract(int dim) {
    int sz = this->size();
    if (dim < -sz) return;
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
    // None is compatible with every tensor
    if (rhs.is_none() || this->is_none()) return true;
    auto us_it = this->value_.end(), them_it = rhs.value_.end();

    do {
      if (*(--us_it) == *(--them_it)) {
        continue;
      } else if (*us_it != 1 && *them_it != 1) {
        return false;
      }
    } while (us_it != this->value_.begin() && them_it != rhs.value_.begin());
    return true;
  }

  void reshape(const std::vector<s_type>& v) {
    if (this->is_none() || this->size() != std::accumulate(v.begin(), v.end(), 1, std::multiplies<s_type>()))
      return;
    value_ = std::vector<s_type>(v);
  }

  void flatten() noexcept {
    if (this->is_none()) return;
    value_ = {this->size()};
  }

  auto value() const { return value_; }
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
  Tensor(const Tensor& rhs) : shape_{rhs.shape_}, value_{rhs.value_} {}
  size_t rank() const { return shape_.rank(); }
  auto size() const { return shape_.size(); }
  void reshape(const std::vector<Shape::s_type>& shape) { shape_.reshape(shape); }
  void flatten() noexcept { shape_.flatten(); } // flatten only affects shape of the tensor, not its value rep
  void extend(int dim) noexcept { shape_.extend(dim); }
  void contract(int dim) noexcept { shape_.contract(dim); }
  bool is_compatible(const Tensor& rhs) const noexcept { return shape_.is_compatible(rhs.shape_); }
  constexpr bool is_none() const { return shape_.is_none(); }

  bool operator > (const Tensor& rhs) const noexcept {
    if (!this->is_compatible(rhs)) return false;
    return this->rank() > rhs.rank();
  }

  Tensor& operator *= (val_type x) noexcept {
    if (this->is_none()) return (*this);
    for (auto& val : value_) val *= x;
    return (*this);
  } // sclar product

  Tensor operator * (val_type x) const noexcept {
    if (this->is_none()) return *this;
    auto res = (*this);
    for (auto& val : res.value_) val *= x;
    return res;
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

    if (this->shape_ == rhs.shape_) { // identical shaped
      Shape shape = this->shape_;
      auto val = this->value_;
      for (auto i = 0; i < val.size(); ++i)
        val[i] += rhs.value_[i];
      return Tensor(shape, val);
    }

    const Tensor& big = (this->rank() > rhs.rank()) ? *this : rhs;
    const Tensor& sml = (&big == this) ? rhs : *this;

    size_t bsz = big.shape_.size(), ssz = sml.shape_.size();
    auto big_sh = big.shape(), sml_sh = sml.shape();
    std::vector<Shape::s_type> shape(big.rank(), 1);
    for (size_t i = 0; i < bsz; ++i) {
      if (i < bsz - ssz) {
        shape[i] = big_sh[i];
      } else {
        shape[i] = std::max(big_sh[i], sml_sh[i + ssz - bsz]);
      }
    }
    Tensor res = Tensor::None();
    return res;
  }

  Tensor operator - (const Tensor& rhs) const noexcept {
    return (*this) + rhs * (-1);
  }

  Tensor operator * (const Tensor& rhs) const noexcept {
    if (!this->is_compatible(rhs)) return Tensor::None();

    if (this->shape_ == rhs.shape_) { // identical
      Shape shape = this->shape_;
      auto val = this->value_;
      for (auto i = 0; i < val.size(); ++i)
        val[i] *= rhs.value_[i];
      return Tensor(shape, val);
    }

    const Tensor& big = (this->rank() > rhs.rank()) ? *this : rhs;
    const Tensor& sml = (&big == this) ? rhs : *this;

    size_t bsz = big.shape_.size(), ssz = sml.shape_.size();
    auto big_sh = big.shape(), sml_sh = sml.shape();
    std::vector<Shape::s_type> shape(big.rank(), 1);
    for (size_t i = 0; i < bsz; ++i) {
      if (i < bsz - ssz) {
        shape[i] = big_sh[i];
      } else {
        shape[i] = std::max(big_sh[i], sml_sh[i + ssz - bsz]);
      }
    }
    Tensor res = Tensor::None();
    return res;
  }

  bool operator == (const Tensor& rhs) const noexcept {
    if (this->is_none() && rhs.is_none()) return true;
    if (!(this->shape_ == rhs.shape_)) return false;
    return this->value_ == rhs.value_;
  }

  auto shape() const { return shape_.value(); }
private:
  Shape shape_;
  std::vector<val_type> value_;
};

} // namespace kl