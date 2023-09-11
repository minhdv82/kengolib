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
    Zeroable(){}
    virtual ~Zeroable() = 0;
    virtual bool is_zero() const = 0;
};

class Moduloable {
  public:
   Moduloable() {};
   virtual Moduloable& operator% (const Moduloable&) = 0;
};

inline void add__(uint64_t& x, uint64_t & y);
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

// primality test employing Fermat's little theorem
// this fails for Carmichael numbers, though ^_^
template <typename T>
bool is_prime(const T& x) {
  if (x == 2) return true;
  T e(2);
  return (pow_mod(e, x, x) == e);
}

// find the first prime number that is greater than x
template <typename T>
T make_prime(const T& x) {
  T val = (x & 1)? x : x + 1;
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
  bigint operator + (const bigint&) const;
  bigint operator - (const bigint&) const;
  bigint operator * (const bigint&) const;
  bigint operator * (uint64_t) const;
  bigint operator / (uint64_t) const;
  bigint operator / (const bigint&) const;
  bigint operator % (const bigint&) const;
  bigint operator >> (int) const;
  bigint operator << (int) const;
  uint64_t operator & (uint64_t x) const { return this->val_[0] & x; }
  bool operator == (const bigint&) const;
  bool operator != (const bigint& rhs) const { return !((*this) == rhs); }
  bool operator == (uint64_t x) const { return (this->size() == 1 && this->val_[0] == x); }
  bool operator != (uint64_t x) const { return !((*this) == x); }
  bool operator > (const bigint&) const;
  bool operator >= (const bigint& rhs) const { return ((*this) == rhs || (*this) > rhs); }
  bool operator < (const bigint& rhs) const {return !((*this) >= rhs); }
  bool operator <= (const bigint& rhs) const { return !((*this) > rhs); }
};
