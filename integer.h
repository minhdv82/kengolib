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

class Moduloable {
  public:
   Moduloable() {};
   virtual Moduloable& operator% (const Moduloable&) = 0;
};

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
T gcd(const T a, const T b) {
  if (b == 0) return a;
  return gcd(b, a % b);
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
  std::vector<uint64_t> val_;
  void display();
  size_t size() const { return val_.size(); }
  bigint() : val_{{0}} {}
  bigint(int x) : val_{static_cast<uint64_t>(x)} {}
  bigint(std::vector<uint64_t> val) : val_{val} {}
  bigint(const bigint& rhs) : val_{rhs.val_} {}
  bigint operator + (const bigint&);
  bigint operator * (const bigint&);
  bigint operator * (uint64_t);
  // bigint operator / (const bigint&);
  // bigint operator % (const bigint&);
  bigint operator >> (int);
  bigint operator << (int);
  bool operator == (const bigint&);
  bool operator == (uint64_t x) { return (this->size() == 1 && this->val_[0] == x); };
  bool operator > (const bigint&);
  // bool operator >= (const bigint&);
  // bool operator < (const bigint&);
  // bool operator <= (const bigint&);
};
