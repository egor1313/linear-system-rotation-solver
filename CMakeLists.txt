cmake_minimum_required(VERSION 3.16)
project(LinearSystemSolver LANGUAGES CXX)

add_executable(linear_solver
    src/main.cpp
)

target_compile_features(linear_solver PRIVATE cxx_std_17)
set_target_properties(linear_solver PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(linear_solver PRIVATE
    -Wall -Wextra -Wpedantic -Wfloat-equal -Werror
    -O3
    -march=native
    -ffast-math
)
