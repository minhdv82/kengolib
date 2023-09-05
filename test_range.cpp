#include <iostream>
#include <assert.h>

#include "range.h"

void test_range() {
  Range<int, int>* range = generate<int, int>(100000, -1);
  range->assign(-1000, 1000, 12345);
  range->assign(-1000, 1000, 12345);
  assert((*range)[-1000] == 12345);
  assert(range->test());

  delete range;
  std::cout << "Test passed!\n" ;
}

int main() {
  test_range();

  return 0;
}