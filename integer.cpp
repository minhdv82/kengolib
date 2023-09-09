
/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */

#include "integer.h"

#include <stdint.h>
#include <iostream>

constexpr void add__(uint64_t& x, uint64_t & y) {
  uint128_t res = x + y;
  x = res;
  y = res >> 64;
}

constexpr void mul__(uint64_t& x, uint64_t& y) {
  uint128_t res = x * y;
  x = res;
  y = res >> 64;
}

bool bigint::operator == (const bigint& rhs) {
  if (this->size() != rhs.size()) return false;
  for (size_t i = 0; i < this->size(); ++i) {
    if (val_[i] != rhs.val_[i]) return false;
  }
  return true;
};

bool bigint::operator == (int rhs) {
  return ((val_.size() == 1) && (val_[0] == static_cast<uint64_t>(rhs)));
};

bigint bigint::operator+ (const bigint& rhs) {
  std::vector<uint64_t> v;
  if (this->size() > rhs.size()) {
    v.resize(this->size());
  } else {
    v.resize(rhs.size());
  }
  uint64_t c = 0;
  int it = 0, vsz = v.size();
  while (it < vsz && it < rhs.size()) {
    v[it] = val_[it];
    uint64_t val = rhs.val_[it];
    add__(v[it], val);
    c += val;
    add__(v[it++], c);
  }

  while (c > 0) {
    std::cout << "vo day nua" << std::endl;
    if (it >= vsz) {
      v.push_back(c);
      break;
    }
    add__(v[it++], c);
  }
  return bigint(v);
}

bigint bigint::operator >> (int n) {
  bigint res = (*this);
  if (n < 1) return res;
  uint64_t c = 0;
  for (int i = this->size() - 1; i >= 0; --i) {
    uint64_t val = (res.val_[i] >> n) | c;
    c = (res.val_[i]) << (64 - n);
    res.val_[i] = val;
  }
  return res;
}

bool bigint::operator > (const bigint& rhs) {
  if (this->size() > rhs.size()){
    return true;
  } else if (this->size() < rhs.size()){
    return false;
  }
  for (int i = rhs.size() - 1; i >= 0; --i) {
    if (this->val_[i] > rhs.val_[i]) return true;
    else if (this->val_[i] < rhs.val_[i]) return false;
  }
  return false;
}