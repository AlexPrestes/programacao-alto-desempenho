#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

//=============================================================================
//
// Definicao de tipos
//

// Codigos de erro do programa.
enum Error { SUCCESS, BAD_ARGUMENT, BAD_FILE };

// Representacao de uma matriz.
class Matrix {
  size_t _nrows, _ncols;
  std::vector<double> _data;

public:
  // Cria uma matriz com o numero de linhas e colunas especificado.
  Matrix(size_t nrows, size_t ncols)
      : _nrows(nrows), _ncols(ncols), _data(nrows * ncols) {}

  size_t num_rows() const { return _nrows; }
  size_t num_cols() const { return _ncols; }

  // Acessa elemento (indexacao) usando (i, j) ao inves de [i, j]
  double &operator()(size_t i, size_t j) { return _data[i * _ncols + j]; }
  double const &operator()(size_t i, size_t j) const {
    return _data[i * _ncols + j];
  }

  // Troca o conteudo de duas matrizes.
  void swap(Matrix &m) {
    std::swap(_nrows, m._nrows);
    std::swap(_ncols, m._ncols);
    _data.swap(m._data);
  }
};

//=============================================================================
//
// Prototipos de funcoes usada no main
//

// Leitura dos argumentos:
// <tamanho, epsilon, arquivo>
std::tuple<size_t, double, std::string> read_args(int argc, char *argv[]);

// Escreve o conteudo da matriz no arquivo especificado.
void write_to_file(std::string const &, Matrix const &);

//=============================================================================
//
// main
//

int main(int argc, char *argv[]) {
  double const outside_temp = 0.;
  double const box_f_value = -100.;

  auto [N, epsilon, filename] = read_args(argc, argv);

  Matrix u(N + 2, N + 2);
  Matrix f(N + 2, N + 2);

  // Ajusta valores nas beiradas.
  for (size_t i = 0; i < N + 2; ++i) {
    u(0, i) = outside_temp;
    u(N + 1, i) = outside_temp;
    u(i, 0) = outside_temp;
    u(i, N + 1) = outside_temp;
  }

  // Calcula o tamanho da caixa interna (onde f e diferente de 0);
  auto box_first = 2 * N / 5 + 1;
  auto box_last = 3 * N / 5;

  // Ajusta valores da matriz de f.
  for (size_t i = box_first; i <= box_last; ++i) {
    for (size_t j = box_first; j <= box_last; ++j) {
      f(i, j) = box_f_value;
    }
  }

  Matrix newu{u};

  auto error{2 * epsilon};

  auto h = 1. / N;

  while (error > epsilon) {
    error = 0.0;

    for (size_t i = 1; i <= N; ++i) {
      for (size_t j = 1; j <= N; ++j) {
        newu(i, j) = 0.25 * (u(i, j - 1) + u(i, j + 1) + u(i - 1, j) +
                             u(i + 1, j) - h * h * f(i, j));
        if (auto this_error = std::fabs(newu(i, j) - u(i, j));
            this_error > error) {
          error = this_error;
        }
      }
    }

    u.swap(newu);
  }

  write_to_file(filename, u);

  return SUCCESS;
}

// Leitura dos argumentos:
// <tamanho, epsilon, arquivo>
std::tuple<size_t, double, std::string> read_args(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Use as: " << argv[0]
              << " <mesh size> <tolerance> <filename>\n";
    std::exit(BAD_ARGUMENT);
  }

  auto N = std::stoul(argv[1]);
  if (N < 1) {
    std::cerr << "Mesh size must be positive.\n";
    std::exit(BAD_ARGUMENT);
  }

  auto epsilon = std::stod(argv[2]);
  if (epsilon <= 0) {
    std::cerr << "Tolerance must be positive.\n";
    std::exit(BAD_ARGUMENT);
  }

  return {N, epsilon, std::string(argv[3])};
}

// Escreve o conteudo da matriz no arquivo especificado.
void write_to_file(std::string const &filename, Matrix const &u) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error opening " << filename << std::endl;
    std::exit(BAD_FILE);
  }

  for (size_t i = 1; i < u.num_rows() - 1; ++i) {
    for (size_t j = 1; j < u.num_cols() - 1; ++j) {
      file << " " << std::setw(14) << u(i, j);
    }
    file << std::endl;
  }

  if (!file.good()) {
    std::cerr << "Error writing to " << filename << std::endl;
    std::exit(BAD_FILE);
  }
}
