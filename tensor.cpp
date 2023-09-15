/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */

#include "tensor.h"

#include <stdint.h>
#include <stdarg.h>

#include <vector>
#include <numeric>

void Shape::reshape(std::vector<s_type> shape) {

}

void Shape::extend(int dim) {
  int sz = this->size();
  if (dim < -sz) return;
  if (dim < 0) dim += sz;
  value_.insert(value_.begin() + dim, 1);
}

void Shape::contract(int dim) {
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

bool Shape::is_compatible(const Shape& rhs) const {
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

