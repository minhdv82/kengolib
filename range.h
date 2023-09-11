/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */
#pragma once

#include "utils.h"

#include <map>

template <typename K, typename V>
class Range {
  private:
    // out of bound value
    V ob_value_;
    std::map<K, V> map_;
  public:
    Range<K, V>(const V& val) : ob_value_{val} {}
    size_t size() const { return map_.size(); }

    void assign(const K& key_from, const K& key_to, const V& val) {
      if (!(key_from < key_to)) return;
      auto it = map_.upper_bound(key_from);
      V res_val = ob_value_;
      if (it != map_.begin()) {
        res_val = (--it)->second;
        ++it;
      }
      if (!(res_val == val)) map_.insert_or_assign(key_from, val);
      while ((it != map_.end()) && (it->first < key_to)) {
        res_val = it->second;
        it = map_.erase(it);
      }
      if (it == map_.end()) {
        if (!(val == ob_value_)) map_.insert_or_assign(key_to, ob_value_);
      } else if ((key_to < it->first) && (!(res_val == val))) {
        map_.insert_or_assign(key_to, res_val);
      }
    }

    const V& operator[] (const K& key) const {
      auto it = map_.upper_bound(key);
      return (it == map_.begin())? ob_value_ : (--it)->second;
    }

    ~Range() {
      map_.clear();
    }

    bool test() const {
      if (map_.size() == 0) return true;
      auto it = map_.begin();
      V val = ob_value_;
      while (it != map_.end()) {
        if (it->second == val) return false;
        val = (it++)->second;
      }
      if ((--it)->second != ob_value_) return false;
      return true;
    }

    void display() const {
      std::cout << "-INFTY : " << ob_value_ << std::endl;
      for (auto [k, v] : map_) {
        std::cout << k << " : " << v << std::endl;
      }
    }
};

template <typename K, typename V>
Range<K, V>* generate(int num_elem, const V& ob_val) {
  Rand rng(82 + time(nullptr));
  Range<K, V>* im = new Range<K, V>{ob_val};
  for (int i = 0; i < num_elem; ++i) {
    int lo = -i + rng.uint32(), hi = rng.uint32() / 3, val = rng.uint32();
    im->assign(static_cast<K>(lo), static_cast<K>(hi), static_cast<V>(val));
  }
  return im;
};