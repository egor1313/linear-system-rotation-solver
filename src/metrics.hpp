#pragma once

#include <cstddef>

#include <cmath>
#include <limits>

template <typename T>
T relative_residual(std::size_t n, const T *a, const T *b, const T *x) {
  T residual_norm = T{0};
  T b_norm = T{0};
  for (std::size_t i = 0; i < n; ++i) {
    T residual = -b[i];
    for (std::size_t j = 0; j < n; ++j) {
      residual += a[i * n + j] * x[j];
    }
    residual_norm = std::hypot(residual_norm, residual);
    b_norm = std::hypot(b_norm, b[i]);
  }
  if (b_norm <= residual_norm * 1e-16) {
    return std::numeric_limits<T>::infinity();
  }
  return residual_norm / b_norm;
}

template <typename T> T solution_error(std::size_t n, const T *x) {
  T norm = T{0};
  for (std::size_t i = 0; i < n; ++i) {
    const T exact = (i % 2 == 0) ? T{1} : T{0};
    norm = std::hypot(norm, x[i] - exact);
  }
  return norm;
}
