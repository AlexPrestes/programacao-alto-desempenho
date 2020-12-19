#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

// Gera valores double aleatórios.
std::vector<double> get_values(size_t const n);

// Calcula uma função "demorada" para cada um dos valores.
std::vector<double> compute_function(std::vector<double> const &data);

// Retorna maior diferença absoluta entre todos os pares dos valores dos dois
// vetores fornecidos (o primero elemento do par vem do primeiro vetor, o
// segundo elemento do segundo vetor).
double all_pairs_max_abs_diff(std::vector<double> const &data1,
                              std::vector<double> const &data2);

// O pragrama lê o tamanho dos vetores da linha de comando.
// Em seguida, calcula os valores iniciais, aplica a função demorada e depois
// calcula a maior diferença absoluta entre pares de valores dos dois vetores.
// As operações de cálculo das funções e cálculo da máxima diferença são
// temporizadas separadamente.
int main(int argc, char *argv[]) {

  // Verifica se o tamanho do vetor foi fornecido na linha de comando.
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " "
              << "<size of vectors>\n";
    return EXIT_FAILURE;
  }

  size_t const N = std::stoul(argv[1]);

  auto t1 = std::chrono::high_resolution_clock::now();

  auto const data_values_1 = get_values(N);
  auto const function_values_1 = compute_function(data_values_1);

  auto const data_values_2 = get_values(N);
  auto const function_values_2 = compute_function(data_values_2);

  auto t2 = std::chrono::high_resolution_clock::now();

  auto const result =
      all_pairs_max_abs_diff(function_values_1, function_values_2);

  auto t3 = std::chrono::high_resolution_clock::now();

  std::cout << "The maximum absolute difference between all pairs is " << result
            << "\n";

  std::cout << "Time taken for computation of function is "
            << (t2 - t1).count() / 1e9 << "\n";
  std::cout << "Time taken for computation of pairs is "
            << (t3 - t2).count() / 1e9 << "\n";

  return EXIT_SUCCESS;
}

// Aqui geramos n valores double aleatórios entre 0 e 1.
std::vector<double> get_values(size_t const n) {
  // Para que os resultados sejam sempre os mesmos para um mesmo tamanho de
  // vetor (facilitando a comparação de resultados), vamos fixar a semente do
  // gerador de números aleatórios. Neste código, isto significa que os dois
  // vetores vão ser iguais, mas não nos importamos com isso aqui.
  std::default_random_engine eg{7600069};
  std::uniform_real_distribution dist;

  std::vector<double> result(n);
  std::generate(begin(result), end(result), [&]() { return dist(eg); });

  return result;
}

// Vamos simular uma função "demorada" usando a função zeta de Riemann
std::vector<double> compute_function(std::vector<double> const &data) {
  std::vector<double> result(data.size());

  std::transform(begin(data), end(data), begin(result),
                 [](double x) { return std::riemann_zeta(x); });

  return result;
}

// Retorna maior diferença absoluta entre todos os pares dos valores no vetor
// fornecido.
double all_pairs_max_abs_diff(std::vector<double> const &data1,
                              std::vector<double> const &data2) {
  double max_abs_diff{0.0};
  for (auto x : data1) {
    for (auto y : data2) {
      auto abs_diff = std::fabs(x - y);
      if (abs_diff > max_abs_diff) {
        max_abs_diff = abs_diff;
      }
    }
  }
  return max_abs_diff;
}
