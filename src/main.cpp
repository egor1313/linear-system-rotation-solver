#include "matrix.hpp"
#include "metrics.hpp"
#include "solver.hpp"

#include <cerrno>
#include <chrono>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

bool str_to_int(const char *str, int &result) {
  char *end;
  errno = 0;
  const long value = std::strtol(str, &end, 10);
  if (errno != 0 || end == str || *end != '\0' || value < 0 ||
      value > INT_MAX) {
    return false;
  }
  result = static_cast<int>(value);
  return true;
}

int main(int ac, char *av[]) {
  using Real = double;

  int res = 0;
  int n, m, k;
  double seconds;
  if (ac != 4 && ac != 5) {
    std::cerr << "To start program write: linear_solver n m k [filename]\n";
    return 1;
  }

  if (!str_to_int(av[1], n) || !str_to_int(av[2], m) || !str_to_int(av[3], k)) {
    std::cerr << "Wrong type of argument\n";
    return 1;
  }
  if (n == 0 || k > 4 || (k == 0 && ac != 5) || (k != 0 && ac != 4)) {
    std::cerr << "I want n > 0, k from 0 to 4, please write filename only if k "
                 "= 0.\n";
    return 1;
  }
  const char *filename = (k == 0) ? av[4] : "";

  const std::size_t size = static_cast<std::size_t>(n);
  if (size > SIZE_MAX / size) {
    std::cerr << "bad size of matrix\n";
    return 1;
  }

  try {
    std::vector<Real> a(size * size);
    initialize_matrix(size, k, filename, a.data());
    std::vector<Real> b(size);
    std::vector<Real> x(size);
    for (std::size_t i = 0; i < size; ++i) {
      b[i] = 0;
      for (std::size_t j = 0; j < size; j += 2) {
        b[i] += a[i * size + j];
      }
    }
    std::vector<Real> right_b = b;

    std::printf("A:\n");
    print_matrix(size, size, a.data(), m);

    auto start = std::chrono::steady_clock::now();
    res = solve(size, a.data(), b.data(), x.data());
    auto end = std::chrono::steady_clock::now();
    if (res != 0) {
      std::cerr << "Cannot solve the system\n";
      return 1;
    }
    seconds = std::chrono::duration<double>(end - start).count();
    std::cout << "Time (in seconds): " << seconds << "\n";
    std::printf("x:\n");
    print_matrix(size, 1, x.data(), m);

    initialize_matrix(size, k, filename, a.data());
    std::cout << "Relative residual: "
              << relative_residual(size, a.data(), right_b.data(), x.data())
              << "\n";
    std::cout << "Solution error: " << solution_error(size, x.data()) << "\n";

  } catch (const std::exception &error) {
    std::fprintf(stderr, "ERROR: %s\n", error.what());
    return 1;
  }
  return 0;
}
