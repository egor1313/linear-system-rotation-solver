#pragma once

#include <cmath>
#include <cstddef>
#include <iostream>
#include <limits>

template <typename T>
void rotate(std::size_t n, T *A, T *B, std::size_t column, std::size_t i,
            std::size_t j) {
  T x = A[i * n + column];
  T y = A[j * n + column];
  if (i == j || std::abs(y) < 1e-16 * std::abs(x)) {
    return;
  }
  T length = std::hypot(x, y);
  T cos_of_phi = x / length;
  T sin_of_phi = y / length;

  T bi = B[i];
  T bj = B[j];
  B[i] = bi * cos_of_phi + bj * sin_of_phi;
  B[j] = -bi * sin_of_phi + bj * cos_of_phi;
  for (std::size_t o = column; o < n; ++o) {
    T ai = A[i * n + o];
    T aj = A[j * n + o];
    A[i * n + o] = ai * cos_of_phi + aj * sin_of_phi;
    A[j * n + o] = -ai * sin_of_phi + aj * cos_of_phi;
  }
}

template <typename T> int solve(std::size_t n, T *A, T *B, T *X) {
  T scale = 0;
  for (std::size_t i = 0; i < n * n; ++i) {
    if (std::abs(A[i]) > scale) {
      scale = std::abs(A[i]);
    }
  }
  T eps = std::numeric_limits<T>::epsilon() * n * scale;

  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = i + 1; j < n; ++j) {
      rotate(n, A, B, i, i, j);
    }
  }
  for (std::size_t i = 0; i < n; ++i) {
    if (std::abs(A[i * n + i]) < eps) {
      std::cout << "Matrix is singular!\n";
      return -1;
    }
  }
  for (std::size_t row = n; row > 0; --row) {
    std::size_t i = row - 1;
    T r = 0;
    for (std::size_t j = i + 1; j < n; ++j) {
      r += A[i * n + j] * X[j];
    }
    X[i] = (B[i] - r) / A[i * n + i];
  }
  return 0;
}
