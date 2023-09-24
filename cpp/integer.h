/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include <stdint.h>

#include <iostream>
#include <vector>

typedef unsigned __int128 uint128_t;

class Zeroable {
 public:
  Zeroable() {}
  virtual ~Zeroable() = 0;
  virtual bool is_zero() const = 0;
};

class Moduloable {
 public:
  Moduloable(){};
  virtual Moduloable& operator%(const Moduloable&) = 0;
};

inline void add__(uint64_t& x, uint64_t& y);
inline void mul__(uint64_t& x, uint64_t& y);

template <typename T>
void swap(T& x, T& y) {
  T t = x;
  x = y;
  y = t;
};

template <typename T>
void swap_x(T& x, T& y) {
  x ^= y;
  y ^= x;
  x ^= y;
}

template <typename T>
T gcd(const T& a, const T& b) {
  if (b == 0) return a;
  return gcd(b, a % b);
};

template <typename T>
void euclid(T& a, T& b) {
  T s = 0, ss = 1, r = a, rr = b;

  while (r != 0) {
    T q = rr / r;
    rr = rr % r;
    swap(r, rr);

    ss = ss - q * s;
    swap(ss, s);
  }

  if (b != 0) {
    b = (rr - ss * a) % b;
  } else {
    b = 0;
  }
  a = ss;
};

// inverse(a, b) * a = gcd(a, b)
template <typename T>
T inverse(const T& a, const T& b) {
  euclid(a, b);

  return (a < 0) ? a + b : a;
};

// compute x^n modulo p
template <typename T>
T pow_mod(const T& x, const T& n, const T& p) {
  T k = n, r = x % p;
  T res = 1;
  while (k > 0) {
    if (k & 1) res = (res * r) % p;
    k = k >> 1;
    r = (r * r) % p;
  }
  return res;
}

template <typename T>
bool miller_rabin(const T& x, int num_witness = 5) {
  const T xmo = x - 1;
  T q = xmo;
  int k = 0;
  while (!(q & 1)) {
    ++k;
    q = q >> 1;
  }

  for (auto w = 2; w < num_witness + 2; ++w) {
    T n = pow_mod(T(w), q, x);
    if (n == 1 || n == xmo) return true;
    for (int i = 0; i < k; ++i) {
      n = (n * n) % x;
      if (n == xmo) return true;
      if (n == 1) return false;
    }
  }

  return false;
}

template <typename T>
bool is_prime(const T& x) {
  return miller_rabin(x);
}

// find the first prime number that is greater than x
template <typename T>
T make_prime(const T& x) {
  T val = (x & 1) ? x : x + 1;
  while (!is_prime(val)) {
    val = val + 2;
  }
  return val;
}

struct bigint {
  std::vector<uint64_t> val_;
  void display() const;
  void canonize() {
    while (val_.back() == 0 && val_.size() > 1) {
      val_.pop_back();
    }
  }
  size_t size() const { return val_.size(); }
  bigint() : val_{{0}} {}
  bigint(uint64_t x) : val_{{x}} {}
  bigint(const std::vector<uint64_t>& val) : val_{val} { canonize(); }
  bigint(bigint&& rhs) : val_{std::move(rhs.val_)} { rhs.val_ = {}; }
  bigint(const bigint& rhs) : val_{rhs.val_} {}
  bigint operator+(const bigint&) const;
  bigint operator-(const bigint&) const;
  bigint operator*(const bigint&) const;
  bigint operator*(uint64_t) const;
  bigint operator/(uint64_t) const;
  bigint operator/(const bigint&) const;
  bigint operator%(const bigint&) const;
  bigint operator>>(int) const;
  bigint operator<<(int) const;
  uint64_t operator&(uint64_t x) const { return this->val_[0] & x; }
  bigint& operator=(const bigint& rhs) {
    this->val_ = rhs.val_;
    return *this;
  }
  bigint& operator=(bigint&& rhs) {
    this->val_ = std::move(rhs.val_);
    rhs.val_ = {};
    return *this;
  }
  bool operator==(const bigint&) const;
  bool operator!=(const bigint& rhs) const { return !(*this == rhs); }
  bool operator==(uint64_t x) const {
    return (this->size() == 1 && this->val_[0] == x);
  }
  bool operator!=(uint64_t x) const { return !(*this == x); }
  bool operator>(const bigint&) const;
  bool operator>=(const bigint& rhs) const {
    return (*this == rhs || *this > rhs);
  }
  bool operator<(const bigint& rhs) const { return !(*this >= rhs); }
  bool operator<=(const bigint& rhs) const { return !(*this > rhs); }
  friend std::ostream& operator<<(std::ostream& oss, const bigint& rhs) {
    oss << rhs.val_.front();
    for (size_t i = 1; i < rhs.val_.size(); ++i) oss << "-" << rhs.val_[i];
    return oss;
  }
};
