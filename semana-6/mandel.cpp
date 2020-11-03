#include <algorithm>
#include <array>
#include <complex>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

enum Error { SUCCESS, BAD_ARGUMENT, BAD_FILE };

using Point = std::complex<double>;       // Um ponto no plano
using Resolution = std::array<size_t, 2>; // Resolucao x (largura), y (altura)

// Classe para representar uma imagem.
// Cada pixel e o numero de iteracoes para detectar |z_n| > 2
class Image {
  std::vector<uint8_t> _values;
  size_t _nrows, _ncols;

public:
  Image(size_t nrows = 0, size_t ncols = 0)
      : _values(nrows * ncols), _nrows(nrows), _ncols(ncols) {}

  void resize(size_t nrows, size_t ncols) {
    _values.resize(nrows * ncols);
    _nrows = nrows;
    _ncols = ncols;
  }

  size_t num_rows() const { return _nrows; }
  size_t num_cols() const { return _ncols; }

  // Operador de funcao e usado para indexacao (linha, coluna).
  uint8_t &operator()(size_t i, size_t j) { return _values[i * _ncols + j]; }

  uint8_t const &operator()(size_t i, size_t j) const {
    return _values[i * _ncols + j];
  }
};

//
// Prototipos de alguma funcoes usadas pelo main.
//

// Escrita de imagem em stream.
std::ostream &operator<<(std::ostream &os, Image const &image);

// Leitura de argumentos de linha de comando.
// Retorna tupla (canto esquerdo inferior, canto direito superior, resolucao).
std::tuple<Point, Point, Resolution> read_args(int argc, char *argv[]);

// Calcula a imagem de Mandelbrot para regiao delimitada por
// lower_left e upper_right, com a resolucao especificada.
Image mandel_block(Point lower_left, Point upper_right, Resolution resolution);

//
// Codigo principal.
//

int main(int argc, char *argv[]) {
  auto [lower_left, upper_right, resolution] = read_args(argc, argv);

  auto image = mandel_block(lower_left, upper_right, resolution);

  std::ofstream output(argv[7]);

  if (!output.is_open()) {
    std::cerr << "Could not open output file.\n";
    return BAD_FILE;
  }
  output << image;

  if (!output.good()) {
    std::cerr << "Error writing to file.\n";
    return BAD_FILE;
  }

  return SUCCESS;
}

std::ostream &operator<<(std::ostream &os, Image const &image) {
  for (size_t i = 0; i < image.num_rows(); ++i) {
    for (size_t j = 0; j < image.num_cols(); ++j) {
      os << static_cast<int>(image(i, j)) << " ";
    }
    os << std::endl;
  }
  return os;
}

std::tuple<Point, Point, Resolution> read_args(int argc, char *argv[]) {
  if (argc < 8) {
    std::cerr << "usage: " << argv[0] << " <lowerleftreal> <lowerleftimag> "
              << " <upperrightreal> <upperrightimag> "
              << " <resolution-x> <resolution-y> "
              << " <outputfilename>\n";
    std::exit(BAD_ARGUMENT);
  }

  double ar = std::stod(argv[1]);
  double ai = std::stod(argv[2]);
  double br = std::stod(argv[3]);
  double bi = std::stod(argv[4]);

  if (ar >= br || ai >= bi) {
    std::cerr << "Position of region delimiting points is wrong.\n";
    std::exit(BAD_ARGUMENT);
  }

  Point lower_left(ar, ai), upper_right(br, bi);

  size_t inx = std::stoul(argv[5]);
  size_t iny = std::stoul(argv[6]);

  if (inx < 1 || iny < 1) {
    std::cerr << "Image resolution is wrong.\n";
    std::exit(BAD_ARGUMENT);
  }

  auto resolution = Resolution{inx, iny};

  return {lower_left, upper_right, resolution};
}

uint8_t mandel(Point c, uint8_t iter_max = 127) {
  uint8_t iter{0};
  Point z{c};

  while (iter < iter_max && abs(z) <= 2.0) {
    ++iter;
    z = z * z + c;
  }

  return iter;
}

Image mandel_block(Point lower_left, Point upper_right, Resolution resolution) {
  double hx = (upper_right.real() - lower_left.real()) / resolution[0];
  double hy = (upper_right.imag() - lower_left.imag()) / resolution[1];

  Image image(resolution[1], resolution[0]);

  for (size_t row = 0; row < resolution[1]; ++row) {
    auto y = lower_left.imag() + row * hy + hy / 2;
    for (size_t col = 0; col < resolution[0]; ++col) {
      auto x = lower_left.real() + col * hx + hx / 2;
      image(row, col) = mandel(Point{x, y});
    }
  }

  return image;
}
