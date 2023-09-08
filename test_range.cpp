/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */
#include <iostream>
#include <assert.h>

#include "range.h"

void test_range() {
  Range<int, int>* range = generate<int, int>(1000000, -1);
  range->assign(-1000, 1000, 12345);
  range->assign(-1000, 1000, 12345);
  assert((*range)[-1000] == 12345);
  assert(range->test());

  // Range<int, char>* range = new Range<int, char>('k');
  // range->assign(-1000, -550, 'k');
  // range->assign(-100, -20, 'c');
  // range->assign(10, 30, 'd');

  range->display();
  assert(range->test());
  delete range;
  std::cout << "Test passed!\n" ;
}

int main() {
  test_range();

  return 0;
}