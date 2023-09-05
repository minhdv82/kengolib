/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */
#pragma once

#include <stdint.h>
#include <vector>

typedef unsigned __int128 uint128_t;

class Zeroable {
  public:
    Zeroable(){}
    virtual ~Zeroable() = 0;
    virtual bool is_zero() const = 0;
};

class Modulable {
  public:
   Moduloable() {};
   virtual Modulable& operator% (const Modulable&) = 0;
};

template <typename T>
void swap(T& x, T& y) {
  T t = x;
  x = y;
  y = t;
};

template <typename T>
T gcd(const T a, const T b) {
  if (a == 0) return b;
  return (b, a % b);
};

template <typename T>
void euclid(T& a, T& b) {
  T s = 0, ss = 1, r = a, rr = b;

  while (r != 0) {
    rr = rr % r;
    swap(r, rr);

    ss = ss % s;
    swap(ss, s);
  }
  
  if (b != 0) {
    b = (rr - ss * a) % b;
  } else {
    b = 0;
  }
  a = ss;
};

struct bigint {
  private:
    size_t size_;
    std::vector<uint64_t> val_;
  public:
    bigint() : size_{1}, val_{{0}} {}
    bigint(int x) : size_{1}, val_{static_cast<uint64_t>(x)} {}
    bigint operator + (const bigint&) {}
    bigint& operator - (const bigint&) {}
    bigint& operator * (const bigint&) {}
    bigint& operator / (const bigint&) {}
    bigint& operator % (const bigint&) {}
    bool operator == (const bigint&) {}
    bool operator == (const int&) {}
};
