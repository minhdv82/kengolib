/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */

#include "integer.h"

#include <stdint.h>

#include <iostream>
#include <cassert>

inline void add__(uint64_t& x, uint64_t & y) {
  uint128_t res = x + y;
  x = res;
  y = res >> 64;
}

inline void mul__(uint64_t& x, uint64_t& y) {
  uint128_t res = x * y;
  x = res;
  y = res >> 64;
}

void bigint::display() {
  for (auto v : val_) std::cout << v << "-";
  std::cout << std::endl;
}

bool bigint::operator == (const bigint& rhs) const {
  if (this->size() != rhs.size()) return false;
  for (size_t i = 0; i < this->size(); ++i) {
    if (val_[i] != rhs.val_[i]) return false;
  }
  return true;
};

bigint bigint::operator+ (const bigint& rhs) const {
  std::vector<uint64_t> v, u;
  if (this->size() > rhs.size()) {
    v = this->val_;
    u = rhs.val_;
  } else {
    v = rhs.val_;
    u = this->val_;
  }
  size_t usz = u.size(), vsz = v.size();
  uint64_t c = 0;
  for (size_t i = 0; i < usz; ++i) {
    add__(v[i], u[i]);
    c += u[i];
    add__(v[i], c);
  }

  size_t i = usz;
  while (c > 0) {
    if (i >= vsz) {
      v.push_back(c);
      break;
    }
    add__(v[i++], c);
  }
  return bigint(v);
}

bigint bigint::operator >> (int n) const {
  std::vector<uint64_t> v = this->val_;
  if (n < 1) return bigint(v);
  if (n == 64) {
    if (v.size() < 2) return 0;
    v.pop_back();
    return bigint(v);
  }
  uint64_t c = 0;
  for (int i = this->size() - 1; i >= 0; --i) {
    uint64_t val = (v[i] >> n) | c;
    c = (v[i]) << (64 - n);
    v[i] = val;
  }

  while (v.back() == 0 and v.size() > 0) {
    v.pop_back();
  }

  return bigint(v);
}

bigint bigint::operator << (int n) const {
  std::vector<uint64_t> v = this->val_;
  if (n == 0) return bigint(v);
  if (n == 64) {
    v.insert(v.begin(), 0);
    return bigint(v);
  }
  uint64_t c = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    uint64_t val = (v[i] << n) | c;
    c = (v[i]) >> (64 - n);
    v[i] = val;
  }
  if (c > 0) v.push_back(c);

  return bigint(v);
}

bool bigint::operator > (const bigint& rhs) const {
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

bigint bigint::operator* (uint64_t x) const {
  if (x == 0) return 0;
  std::vector<uint64_t> v = this->val_;
  uint64_t c = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    uint64_t val = x;
    mul__(v[i], val);
    add__(v[i], c);
    c += val;
  }
  if (c > 0) {
    v.push_back(c);
  }
  return bigint(v);
}

bigint bigint::operator* (const bigint& rhs) const {
  bigint val = (*this);
  bigint res = val * rhs.val_.back();
  for (int i = rhs.size() - 2; i >= 0; --i) {
    res = (res << 64) + (val) * rhs.val_[i];
  }
  return res;
}

bigint bigint::operator / (uint64_t x) const {
  assert(x != 0);
  if ((*this) < x) return 0;
  std::vector<uint64_t> v;
  uint128_t x128 = static_cast<uint128_t>(x);
  uint64_t c = 0;
  for (int i = this->size() - 1; i >= 0; --i) {
    uint128_t val = (static_cast<uint128_t>(c) << 64) | static_cast<uint128_t>(this->val_[i]);
    c = static_cast<uint64_t>(val % x128);
    uint64_t earl = static_cast<uint64_t>(val / x128);
    // std::cout << "earl: " << earl << std::endl;
    v.insert(v.begin(), earl);
  }

  // for (auto e : v) {
  //   std::cout << "ee : " << e << std::endl;
  // }

  while (v.back() == 0 && v.size() > 1) {
    v.pop_back();
  }

  return bigint(v);

}

// bigint bigint::operator/ (const bigint& rhs) const {
//   assert(rhs != 0);

// }

bigint bigint::operator % (uint64_t x) const {
  assert(x != 0);
  if ((*this) < x) return (*this);

  uint64_t c = 0;
  uint128_t xx = x;
  for (int i = this->size() - 1; i >= 0; --i) {
    uint128_t val = (static_cast<uint128_t>(c) << 64) | static_cast<uint128_t>(this->val_[i]);
    c = static_cast<uint64_t>(val % xx);
  }
  return bigint({c});
 }  
  