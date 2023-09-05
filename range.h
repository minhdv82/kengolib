/*
 * Copyright (c) [2023] Minh v. Duong; minhdv82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */
#pragma once
#include <map>

template <typename K, typename V>
class Range {
  private:
    // out of bound value
    V ob_value_;
    std::map<K, V> range_;
  public:
    Range<K, V>(const V& val) : ob_value_{val} {}
    size_t size() const { return range_.size(); }

    void assign(const K& key_from, const K& key_to, const V& val) {
      if (!(key_from < key_to)) return;
      auto it = range_.upper_bound(key_from);
      V res_val = ob_value_;
      if (it != range_.begin()) {
        res_val = (--it)->second;
        ++it;
      }
      if (!(res_val == val)) range_.insert_or_assign(key_from, val);
      while ((it != range_.end()) && (it->first < key_to)) {
        res_val = it->second;
        it = range_.erase(it);
      }
      if (it == range_.end()) {
        if (!(val == ob_value_)) range_.insert_or_assign(key_to, ob_value_);
      } else if ((key_to < it->first) && (!(res_val == val))) {
        range_.insert_or_assign(key_to, res_val);
      }
    }

    const V& operator[] (const K& key) const {
      auto it = range_.upper_bound(key);
      return (it == range_.begin())? ob_value_ : (--it)->second;
    }

    ~Range() {
      range_.clear();
    }

    bool test() const {
      if (range_.size() == 0) return true;
      auto it = range_.begin();
      V val = ob_value_;
      while (it != range_.end()) {
        if (it->second == val) return false;
        val = (it++)->second;
      }
      if ((--it)->second != ob_value_) return false;
      return true;
    }
};

template <typename K, typename V>
Range<K, V>* generate(int num_elem, const V val) {
  #include <time.h>
  #include <stdlib.h>
  srand(time(NULL) + 137);
  Range<K, V>* im = new Range<K, V>{val};
  for (int i = 0; i < num_elem; ++i) {
    int lo = -i + std::rand(), hi = std::rand() / 3, val = std::rand();
    im->assign(static_cast<K>(lo), static_cast<K>(hi), static_cast<V>(val));
  }
  return im;
};