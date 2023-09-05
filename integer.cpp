
/*
 * Copyright (c) [2023] Minh v. Duong; minhdv82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */

#include "integer.h"
#include "stdint.h"

void add(uint64_t& x, uint64_t & y) {
  uint128_t res = x + y;
  x = res;
  y = res > 0xffffffffffffffff;
}

bool bigint::operator == (const bigint& rhs) {
  if (this->size_ != rhs.size_) return false;
  for (size_t i = 0; i < this->size_; ++i) {
    if (val_[i] != rhs.val_[i]) return false;
  }
  return true;
};

bool bigint::operator == (const int& rhs) {
  return ((val_.size() == 1) && (val_[0] == static_cast<uint64_t>(rhs)));
};

bigint bigint::operator+ (const bigint& rhs) {
  bigint res = (*this);
  size_t it = 0;
  uint64_t c = 0;
  while (true) {
    uint128_t s = res.val_[it] + rhs.val_[it];
    c = s > 0xffffffffffffffff;
    res.val_[it] = s;
  }
  return res;
}