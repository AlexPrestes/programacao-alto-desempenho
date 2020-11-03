#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace chrono = std::chrono;

// Hand-mplemented quicksort of a vector of ints (for comparison)
void quick_sort(std::vector<int> &);

// Custom function to read command line arguments
std::pair<size_t, int> read_arguments(int, char *[]);

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

  for (int j = 0; j < M; ++j) {

    std::generate(begin(rnd_array), end(rnd_array), draw);

    auto t1 = chrono::high_resolution_clock::now();

    quick_sort(rnd_array);

    auto t2 = chrono::high_resolution_clock::now();

    auto dt = chrono::duration_cast<chrono::microseconds>(t2 - t1);
    elapsed += dt.count();
  }

  // Show timing resuts.
  std::cout << N << " " << elapsed / M / 1e6 << std::endl;
}

using vector_int_it = std::vector<int>::iterator;

/**
 * Sort elements from ini (included) to fin (excluded) using
 * quicksort.
 */
void quick_sort(vector_int_it const &ini, vector_int_it const &fin) {
  auto interval_size = fin - ini;
  if (interval_size > 2) {

    // Choose first element as key
    int key = *ini;

    // Split elements according to key
    auto end_small = ini;
    auto begin_large = fin;
    auto end_equal = ini + 1;
    while (end_equal != begin_large) {
      // In [ini,end_small) all are < key
      // In [end_small, end_equal) all are == key
      // In [begin_large, fin) all are > key
      // In [end_equal, begin_large) are unknown
      // *end_equal != key

      // Put value at end_equal at the right place
      if (*end_equal < key) {
        std::iter_swap(end_equal, end_small);
        // Ranges of smallers increased by one, range of
        // equals shifted.
        ++end_small;
        ++end_equal;
      } else if (*end_equal > key) {
        std::iter_swap(end_equal, begin_large - 1);
        --begin_large;
      } else {
        // *end_equal == key
        ++end_equal;
      }
    }

    // Recursive call.  The two are garateed to be smaller than
    // the original, as the value(s) equal to key are ignored.
    quick_sort(ini, end_small);
    quick_sort(begin_large, fin);
  } else if (interval_size == 2 && *ini > *(ini + 1)) {
    // The two elements are out of order. Swap.
    std::iter_swap(ini, ini + 1);
  }
}

/**
 * Sort all elements of array v using quick sort.
 */
void quick_sort(std::vector<int> &v) { quick_sort(begin(v), end(v)); }

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
