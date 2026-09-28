#pragma once

#include <cstddef>
#include <string>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <stdexcept>

template <typename T>
T formula(int k, std::size_t n, std::size_t i, std::size_t j) {
  switch (k) {
  case 1:
    return static_cast<T>(n - std::max(i, j) + 1);
  case 2:
    return static_cast<T>(std::max(i, j));
  case 3:
    return static_cast<T>(std::max(i, j) - std::min(i, j));
  case 4:
    return T{1} / (static_cast<T>(i) + static_cast<T>(j) - T{1});
  default:
    throw std::invalid_argument("The formula number must be from 1 to 4");
  }
}

template <typename T>
void read_matrix(const std::string &filename, std::size_t n, T *a) {
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("Cant open file: " + filename);
  }
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      T value = T{0};
      if (!(input >> value) || !std::isfinite(value)) {
        throw std::runtime_error("Bad data in matrix");
      }
      a[i * n + j] = value;
    }
  }
  input >> std::ws;
  if (!input.eof()) {
    throw std::runtime_error("I can see unnecessary data after matrix in file");
  }
}

template <typename T>
void initialize_matrix(std::size_t n, int k, const std::string &filename,
                       T *a) {
  if (k == 0) {
    read_matrix(filename, n, a);
    return;
  }
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      a[i * n + j] = formula<T>(k, n, i + 1, j + 1);
    }
  }
}

template <typename T>
void print_matrix(std::size_t rows, std::size_t cols, const T *a,
                  std::size_t m) {
  for (std::size_t i = 0; i < std::min(rows, m); ++i) {
    for (std::size_t j = 0; j < std::min(cols, m); ++j) {
      std::printf(" %10.3e", static_cast<double>(a[i * cols + j]));
    }
    std::printf("\n");
  }
}
