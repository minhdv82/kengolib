/*
 * Copyright (c) [2023] Minh v. Duong; dvminh82@gmail.com
 *
 * You are free to use, modify, re-distribute this code at your own risk
 */
#pragma once

#include <iostream>

template <typename T>
T npow(T x, int n) {
  if (n == 0) return 1;
  int nn = n > 0 ? n : -n;
  T res = x;
  for (int i = 0; i < nn; i++) {
    res *= x;
  }
  if (n < 0) res = 1 / res;
  return res;
}

template <typename T>
inline T ABS(T x) {
  return x > 0.0 ? x : -x;
}

template <typename T>
void swaprow(const int n, const int i1, const int i2, T* A) {
  for (int col = 0; col < n; col++) {
    T tmp = A[i1 + n * col];
    A[i1 + n * col] = A[i2 + n * col];
    A[i2 + n * col] = tmp;
  }
}

template <typename T>
void swaprowT(const int n, const int i1, const int i2, T* AT) {
  for (int col = 0; col < n; col++) {
    T tmp = AT[i1 * n + col];
    AT[i1 * n + col] = AT[i2 * n + col];
    AT[i2 * n + col] = tmp;
  }
}

// LU decomposition with partial pivoting
// A[i][j] = A[i + n*j] is the matrix to be LU decomped
// perm is the permutation matrix, perm[1] = 2 means move row 1 of A to row 2 etc
template <typename T>
void pludec(const int n, T* A, int* perm) {
  if (n < 2) return;
  int i, j, k, imax;
  T piv, tmax, tmp;
  T* TA = new T[n * n];
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) TA[i * n + j] = A[i + n * j];  // TA is A but with "transposed" storing

  int* index = new int[i];
  for (i = 0; i < n; i++) index[i] = i;

  for (i = 0; i < n; i++) {
    // pick pivot
    tmax = -1.0;
    imax = i;
    int ni = n * i;
    for (j = i; j < n; j++) {
      tmp = 0.0;
      for (k = 0; k < i; k++) tmp += TA[j * n + k] * A[k + ni];
      tmp = A[j + ni] - tmp;

      if (ABS<T>(tmp) > tmax) {
        tmax = ABS<T>(tmp);
        imax = j;
      }
    }
    perm[i] = index[imax];  // index[imax] row of original A is mapped to row i of LU
    perm[n + index[imax]] = i;

    // swap rows
    if (i != imax) {
      int idx = index[imax];
      index[imax] = index[i];
      index[i] = idx;
      swaprow<T>(n, i, imax, A);
      swaprowT<T>(n, i, imax, TA);
    }

    tmp = 0.0;
    for (k = 0; k < i; k++) tmp += TA[ni + k] * A[k + ni];
    A[i + ni] -= tmp;  // first calc. the pivot
    piv = A[i + ni];
    TA[ni + i] = A[i + ni];  // update TA too

    // now update column i from row i+1
    for (int row = i + 1; row < n; ++row) {
      tmp = 0.0;
      for (k = 0; k < i; k++) tmp += TA[row * n + k] * A[k + ni];
      A[row + ni] = (A[row + ni] - tmp) / piv;
      TA[row * n + i] = A[row + ni];
    }

    for (int col = i + 1; col < n; col++) {
      tmp = 0.0;
      for (k = 0; k < i; k++) tmp += TA[ni + k] * A[k + n * col];
      A[i + n * col] -= tmp;
      TA[ni + col] = A[i + n * col];
    }
  }
  delete[] index;
  delete[] TA;
}

// decompose A = LU
// note A[i][j] = A[i + n*j]
template <typename T>
void ludec(const int n, T* A) {
  int i, j, k;
  T* Aik = new T[n];
  T sum;
  for (i = 1; i < n; i++) {
    for (k = 0; k < n; k++) Aik[k] = A[i + n * k];
    for (j = 0; j < n; j++) {
      sum = 0.0;
      if (i > j) {  // cal L
        for (k = 0; k < j; k++) sum += Aik[k] * A[k + n * j];
        A[i + n * j] = (A[i + n * j] - sum) / A[j + n * j];
      } else {  // cal U
        for (k = 0; k < i; k++) sum += Aik[k] * A[k + n * j];
        A[i + n * j] -= sum;
      }
      Aik[j] = A[i + n * j];  // update
    }
  }
  delete[] Aik;
}

template <typename T>
T det(const int n, const T* A) {
  // if (same_type<T>())
  // 	mpreal::set_default_prec(prec);
  T res = 0.0;
  int i;
  T* tA = new T[n * n];
  for (i = 0; i < n * n; i++) tA[i] = A[i];
  ludec<T>(n, tA);
  res = 1.0;
  for (i = 0; i < n; i++) res *= tA[i + n * i];
  delete[] tA;
  return res;
}

// solve for Ax = y with lower triangle A
// A[i][j] = A[i * n + j]
template <typename T>
void lowsub(const int n, const T* A, T* x, const T* y) {
  T sum;
  x[0] = y[0] / A[0];
  for (int row = 1; row < n; row++) {
    sum = 0.0;
    for (int col = 0; col < row; col++) sum += A[row * n + col] * x[col];
    x[row] = (y[row] - sum) / A[row * n + row];
  }
}

// solve for Ax = y with lower unit triangle A A(ii) = 1
// A[i][j] = A[i*n + j]
template <typename T>
void low1sub(const int n, const T* A, T* x, const T* y) {
  T sum;
  x[0] = y[0];
  for (int row = 1; row < n; row++) {
    sum = 0.0;
    for (int col = 0; col < row; col++) sum += A[row * n + col] * x[col];
    x[row] = y[row] - sum;
  }
}

// solve for Ax = y with upper triangle A
// A[i][j] = A[i*n + j]
template <typename T>
void upsub(const int n, const T* A, T* x, const T* y) {
  T sum;
  x[n - 1] = y[n - 1] / A[n * n - 1];
  for (int row = n - 2; row >= 0; row--) {
    sum = 0.0;
    for (int col = row + 1; col < n; col++) sum += A[row * n + col] * x[col];
    x[row] = (y[row] - sum) / A[row * n + row];
  }
}

// take inverse of A
// A[i][j] = A[i + n*j]
template <typename T>
void pluinverse(const int n, T* A) {  // use pludec
  int row, col;
  int* perm = new int[2 * n];
  T* tA = new T[n * n];
  T* tAA = new T[n * n];
  T* tcol = new T[n];
  T* icol = new T[n];
  pludec<T>(n, A, perm);  // now PA = LU

  // store LU of PA to tA with 'transposed' storage
  for (int row = 0; row < n; row++)
    for (int col = 0; col < n; col++) tA[row * n + col] = A[row + n * col];

  for (col = 0; col < n; col++) {
    for (row = 0; row < n; row++) icol[row] = 0.0;
    icol[col] = 1.0;
    low1sub<T>(n, tA, tcol, icol);                                 // now tcol is UA**-1
    upsub<T>(n, tA, icol, tcol);                                   // now icol is A**-1
    for (row = 0; row < n; row++) tAA[row + n * col] = icol[row];  // update icol to A**-1
  }                                                                // now tAA is inverse of LU

  // assign A ---> AP
  for (col = 0; col < n; col++) {
    int lucol = perm[n + col];
    for (row = 0; row < n; row++) A[row + n * col] = tAA[row + n * lucol];
  }
  delete[] tA;
  delete[] tAA;
  delete[] tcol;
  delete[] icol;
  delete[] perm;
}

// solve AX = B for matrix X
// where A is n by n, B and X are n by m matrices
template <typename T>
void plusolve(const int n, const T* A, const int m, T* X, const T* B) {
  int row, col;
  int* perm = new int[2 * n];
  T* tA = new T[n * n];
  T* tAA = new T[n * n];
  T* tcol = new T[n];
  T* icol = new T[n];

  for (int i = 0; i < n * n; i++) tAA[i] = A[i];
  pludec<T>(n, tAA, perm);  // now PA = LU

  // store LU of PA to tA with 'transposed' storage
  for (int row = 0; row < n; row++)
    for (int col = 0; col < n; col++) tA[row * n + col] = tAA[row + n * col];

  for (col = 0; col < m; col++) {
    for (row = 0; row < n; row++) icol[perm[n + row]] = B[row + n * col];  // get P*b_col
    low1sub<T>(n, tA, tcol, icol);                                         // solve for L*y = P*b_col
    upsub<T>(n, tA, icol, tcol);                                           // solve for U*x_col = y
    for (row = 0; row < n; row++) X[row + n * col] = icol[row];            // store x_col
  }

  delete[] tA;
  delete[] tAA;
  delete[] tcol;
  delete[] icol;
  delete[] perm;
}

// take inverse of A
// A[i][j] = A[i + n*j]
template <typename T>
void luinverse(const int n, T* A) {
  int row, col;
  T* tA = new T[n * n];
  T* tcol = new T[n];
  T* icol = new T[n];
  ludec<T>(n, A);
  for (int row = 0; row < n; row++)
    for (int col = 0; col < n; col++) tA[row * n + col] = A[row + n * col];

  for (col = 0; col < n; col++) {
    for (row = 0; row < n; row++) icol[row] = 0.0;
    icol[col] = 1.0;
    low1sub<T>(n, tA, tcol, icol);                               // now tcol is UA**-1
    upsub<T>(n, tA, icol, tcol);                                 // now icol is A**-1
    for (row = 0; row < n; row++) A[row + n * col] = icol[row];  // update icol to A**-1
  }
  delete[] tA;
  delete[] tcol;
  delete[] icol;
}

// linear least squares fit y = sum_c[i]*f[i](x)
// return vector of coefficients c
template <typename T>
void lls(const int m, const int n, const T* _f, const T* _y, T* coefs) {
  if (n < m) {
    std::cout << "Invalid LLS parameters!\n";
    return;
  }

  T* f = new T[m * m];
  T* y = new T[m];

  for (int a = 0; a < m; ++a) {
    y[a] = 0.0;
    for (int i = 0; i < n; ++i) y[a] += _y[i] * _f[a + m * i];
  }

  for (int a = 0; a < m; ++a) {
    f[a + m * a] = 0.0;
    for (int i = 0; i < n; ++i) f[a + m * a] += _f[a + m * i] * _f[a + m * i];
    for (int b = a + 1; b < m; ++b) {
      f[a + m * b] = 0.0;
      for (int i = 0; i < n; ++i) f[a + m * b] += _f[a + m * i] * _f[b + m * i];
      f[b + m * a] = f[a + m * b];
    }
  }

  luinverse<T>(m, f);
  for (int a = 0; a < m; ++a) {
    coefs[a] = 0.0;
    for (int b = 0; b < m; ++b) coefs[a] += f[a + m * b] * y[b];
  }

  delete[] f;
  delete[] y;
}

template <typename T>
void polyfit(const int deg, const int n, const T* x, const T* f, T* coefs) {
  if (deg >= n) {
    std::cout << "Invalid parameters in polyfit!\n";
    return;
  }

  T* u = new T[deg];
  T* M = new T[deg * deg];

  for (int a = 0; a < deg; ++a) {
    T val = 0.0;
    for (int i = 0; i < n; ++i) val += npow<T>(x[i], a * 2);
    M[a + deg * a] = val;
    for (int b = a + 1; b < deg; ++b) {
      T earl = 0.0;
      for (int i = 0; i < n; ++i) earl += npow<T>(x[i], a + b);
      M[a + deg * b] = earl;
      M[b + deg * a] = earl;
    }
  }

  for (int a = 0; a < deg; ++a) {
    T val = 0.0;
    for (int i = 0; i < n; ++i) val += npow<T>(x[i], a) * f[i];
    u[a] = val;
  }

  luinverse<T>(deg, M);
  for (int a = 0; a < deg; ++a) {
    T val = 0.0;
    for (int b = 0; b < deg; ++b) val += M[a + deg * b] * u[b];
    coefs[a] = val;
  }

  delete[] u;
  delete[] M;
}

// fit y = A * x^n where c[0] = A and c[1] = n
// using linear least squares lls
template <typename T>
void powerfit(const int n, const T* _x, const T* _y, T* coefs) {
  int m = 2;
  T* y = new T[n];

  for (int i = 0; i < n; ++i) {
    y[i] = T::log(_y[i]);
  }

  T* f = new T[m * n];
  for (int i = 0; i < n; ++i) {
    f[0 + 2 * i] = 1.0;
    f[1 + 2 * i] = T::log(_x[i]);
  }

  lls<T>(m, n, f, y, coefs);

  delete[] y;
  delete[] f;
}