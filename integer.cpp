/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */

#include "integer.h"

#include <stdint.h>

#include <iostream>

inline void add__(uint64_t& x, uint64_t& y) {
  uint128_t val = static_cast<uint128_t>(x) + static_cast<uint128_t>(y);
  x = val;
  y = val >> 64;
}

inline void sub__(uint64_t& x, uint64_t& y) {
  uint128_t val = static_cast<uint128_t>(x);
  if (val >= y) {
    x = val - y;
    y = 0;
  } else {
    x = val + (uint128_t(1) << 64) - y;
    y = 1;
  }
}

/*
This subroutine multiplies two unsigned 64 bit
then stores the result and carry
*/
inline void mul__(uint64_t& x, uint64_t& y) {
  uint128_t val = static_cast<uint128_t>(x) * static_cast<uint128_t>(y);
  x = val;
  y = val >> 64;
}

void bigint::display() const {
  std::cout << val_.front();
  for (size_t i = 1; i < val_.size(); ++i) std::cout << "-" << val_[i];
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
  size_t i = 0;
  for (i = 0; i < usz; ++i) {
    add__(v[i], u[i]);
    add__(v[i], c);
    c += u[i];
  }

  while (c > 0) {
    if (i >= vsz) {
      v.push_back(c);
      break;
    }
    add__(v[i++], c);
  }
  return bigint(v);
}

bigint bigint::operator - (const bigint& rhs) const {
  if (*this <= rhs) return 0;
  std::vector<uint64_t> v = val_, u = rhs.val_;
  size_t usz = u.size(), vsz = v.size();
  uint64_t c = 0;
  size_t i;
  for (i = 0; i < usz; ++i) {
    sub__(v[i], u[i]);
    sub__(v[i], c);
    c += u[i];
  }

  while (c > 0) {
    if (i >= vsz) {
      v.push_back(c);
      break;
    }
    sub__(v[i++], c);
  }
  return bigint(v);
}

bigint bigint::operator >> (int n) const {
  if (n < 1 || *this == 0) return *this;
  std::vector<uint64_t> v = val_;
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

  return bigint(v);
}

bigint bigint::operator << (int n) const {
  if (n == 0 || *this == 0) return *this;
  std::vector<uint64_t> v = val_;
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
    if (val_[i] > rhs.val_[i]) return true;
    else if (val_[i] < rhs.val_[i]) return false;
  }
  return false;
}

bigint bigint::operator* (uint64_t x) const {
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
  bigint res = rhs * val_.back();
  for (int i = val_.size() - 2; i >= 0; --i) {
    res = (rhs * val_[i]) + (res << 64);
  }
  return res;
}

bigint bigint::operator / (uint64_t x) const {
  if (*this < x || x <= 1 ) return 0;
  if (*this == x) return 1;
  size_t sz = val_.size();
  std::vector<uint64_t> v(sz, 0);
  uint64_t c = 0;
  for (int i = sz - 1; i >= 0; --i) {
    uint128_t val = (static_cast<uint128_t>(c) << 64) + static_cast<uint128_t>(val_[i]);
    c = static_cast<uint64_t>(val % x);
    v[i] = static_cast<uint64_t>(val / x);
  }

  return bigint(v);
}

void divmod__(bigint& no, bigint& de) {
  bigint earl = 0;
  bigint val = no;
  while (val >= de) {
    if (val == de) {
      no = earl + 1;
      de = 0;
      return;
    }
    uint128_t v = val.val_.back(), u = de.val_.back();
    if (v < u) {
      v = (v << 64) + val.val_[val.size() - 2];
    }
    uint64_t sig = v / u;
    if (sig > 1) {
      --sig;
    }
    size_t n = val.size() - de.size();
    std::vector<uint64_t> vv;
    for (size_t i = 0; i < n; ++i) vv.push_back(0);
    vv.push_back(sig);
    bigint r(vv);

    while (r * de < val) {
      r = r << 1;
    }

    while (r * de > val) {
      r = r >> 1;
    }

    val = val - r * de;
    earl = earl + r;
  }
  no = earl;
  de = val;
}

bigint bigint::operator / (const bigint& rhs) const {
  bigint no = bigint(this->val_), de = bigint(rhs.val_);
  divmod__(no, de);
  return no;
}

bigint bigint::operator % (const bigint& rhs) const {
  bigint no = bigint(this->val_), de = bigint(rhs.val_);
  divmod__(no, de);
  return de;
}
  