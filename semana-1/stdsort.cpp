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
int main(int argc, char *argv[]) {
  // Read N and M from command line
  auto [N, M] = read_arguments(argc, argv);

  // Function for random number generation
  std::random_device entropy;
  std::mt19937 gen(entropy()); // Randomness generator.
  std::uniform_int_distribution<> dis(0, 1000 * N);
  auto draw = [&gen, &dis]() { return dis(gen); };

  std::vector<int> rnd_array(N);

  // Time sorting M random arrays of N elements.
  double elapsed = 0;

  for (int j = 0; j < M; j++) {

    std::generate(begin(rnd_array), end(rnd_array), draw);

    auto t1 = chrono::high_resolution_clock::now();

    std::sort(begin(rnd_array), end(rnd_array));

    auto t2 = chrono::high_resolution_clock::now();

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
