/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk.
 */

#include "utils.h"

#include <time.h>

#include <iostream>

int main() {
  Rand rng(42 + time(nullptr));
  
  std::cout << rng.doub() << std::endl;

  return 0;
}