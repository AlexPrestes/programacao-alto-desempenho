#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace chrono = std::chrono;

// Custom function to read command line arguments
std::pair<size_t, int> read_arguments(int argc, char *argv[]);

/**
 * Read number of elements (N) from the command line, generate a
 * random vector of this size and sort it.
 * Repeat M times for statistics (M is the second command line argument).
 */

struct Matrix {
  std::vector<double> _store;
  size_t _N;
  Matrix(size_t N) : _store(N * N), _N(N) {}

  double &operator()(size_t i, size_t j) { return _store[i * _N + j]; }
  double const &operator()(size_t i, size_t j) const {
    return _store[i * _N + j];
  }
};

Matrix rnd_matrix(std::mt19937 &gen, size_t N) {
  std::uniform_real_distribution<double> dis(0, 1);
  Matrix m(N);
  std::generate(&m(0, 0), &m(N - 1, N), [&gen, &dis]() { return dis(gen); });
  return m;
}

int main(int argc, char *argv[]) {
  // Read N and M from command line
  auto [N, M] = read_arguments(argc, argv);

  std::mt19937 gen(std::random_device{}()); // Randomness generator.

  // Time sorting M random arrays of N elements.
  double elapsed = 0;
  chrono::high_resolution_clock::time_point t1, t2;

  for (int j = 0; j < M; j++) {

    Matrix a = rnd_matrix(gen, N);
    Matrix b = rnd_matrix(gen, N);
    Matrix c(N);

    t1 = chrono::high_resolution_clock::now();

    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < N; ++j) {
        c(i, j) = a(i, j) + b(i, j);
      }
    }

    t2 = chrono::high_resolution_clock::now();

    auto dt = chrono::duration_cast<chrono::microseconds>(t2 - t1);
    elapsed += dt.count();
  }

  // Show timing resuts.
  std::cout << N << " " << elapsed / M / 1e6 << std::endl;
}

std::pair<size_t, int> read_arguments(int argc, char *argv[]) {
  // We need exactly three arguments (considering program name).
  if (argc != 3) {
    std::cout << "Usage: " << argv[0]
              << " <number of elements> <number of arrays>\n";
    std::exit(1);
  }

  // Read arguments.
  size_t N = std::stoul(argv[1]);
  if (N < 1) {
    std::cerr << "There must be at least 1 element in the array.\n";
    std::exit(1);
  }

  int M = std::stoi(argv[2]);
  if (M < 1) {
    std::cerr << "We must generate at least one array.\n";
    std::exit(1);
  }

  return {N, M};
}
