#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

enum Error { SUCCESS, BAD_ARGUMENT };

using Vector = std::vector<double>;

class Matrix {
  std::vector<double> _storage;
  size_t _nrows, _ncols;

public:
  Matrix(size_t nrows = 0, size_t ncols = 0)
      : _storage(nrows * ncols), _nrows(nrows), _ncols(ncols) {}

  size_t nrows() const { return _nrows; }

  size_t ncols() const { return _ncols; }

  double &operator()(size_t i, size_t j) { return _storage[i * _ncols + j]; }

  double const &operator()(size_t i, size_t j) const {
    return _storage[i * _ncols + j];
  }
};

Vector matrix_vector_product(Matrix const &m, Vector const &v);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Give the size of the vectors in "
                 "the command line.\n";
    return BAD_ARGUMENT;
  }

  size_t N = std::stoul(argv[1]);
  if (N < 1) {
    std::cerr << "The number of elements must be positive.\n";
    return BAD_ARGUMENT;
  }

  Vector v(N);
  Matrix m(N, N);

  // Random number generation
  std::random_device rd;
  std::default_random_engine rand_gen(rd());
  std::uniform_real_distribution<double> dist(0, 1);
  auto gen_rand_real = [&]() { return dist(rand_gen); };

  std::generate(begin(v), end(v), gen_rand_real);

  std::generate(&m(0, 0), &m(m.nrows() - 1, m.ncols()), gen_rand_real);

  Vector vres = matrix_vector_product(m, v);

  double s{0.0};
  for (size_t i = 0; i < N; i++) {
    s += vres[i];
  }

  std::cout << std::setprecision(15);
  std::cout << "The sum of the resulting vector is " << s << std::endl;

  return SUCCESS;
}

Vector matrix_vector_product(Matrix const &m, Vector const &v) {
  Vector res(m.nrows());

  for (size_t i = 0; i < m.nrows(); ++i) {
    double s{0.0};
    for (size_t j = 0; j < m.ncols(); ++j) {
      s += m(i, j) * v[j];
    }
    res[i] = s;
  }

  return res;
}
