#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>

void const_e_referencias() {
  int a;            // variável normal, não inicializida
  int b = 1;        // variável normal, inicializada
  int c{1};         // idem a b
  int const d = 42; // constante; precisa inicialização
  // int const d; seria um erro
  const int e{42}; // idem a d
  std::cout << "a, b, c, d, e = " << a << " " << b << " " << c << " " << d
            << " " << e << std::endl;

  int &ra = a; // Referência (sinônimo) para a
  ra = 7;      // Muda o valor de a para 7

  int const &rd{d}; // referência constante para a constante d
  int const &rc{c}; // referência constante para a variável c
  b += rc;          // soma 1 em b

  std::cout << "ra, a, rd, d, rc, c, b = " << ra << " " << a << " " << rd << " "
            << d << " " << rc << " " << c << " " << b << std::endl;
  int *pa = &a; // ponteiro para a variável a
  *pa += 2;     // Muda a para 9

  int const *pb = &b; // Ponteiro para b, constante
  c = *pb;            // OK
  // *pb = e; seria um erro

  std::cout << "*pa, a, *pb, b, c = " << *pa << " " << a << " " << *pb << " "
            << b << " " << c << std::endl;

  int constexpr duzia{12};  // Constante de tempo de compilação
  constexpr int dezena{10}; // constante de tempo de compilação

  std::cout << "duzia, dezena = " << duzia << " " << dezena << std::endl;
}

void arranjos_lineares() {
  int v1[10]; // 10 inteiros consecutivos na memória
  // Não é comumento usado em C++

  std::cout << "v1 vale: ";
  std::copy(std::begin(v1), std::end(v1),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  std::array<int, 10> v2; // 10 arrays consecutivos na memória
  // Forma C++ de fazer a declaração

  std::cout << "v2 vale: ";
  std::copy(begin(v2), end(v2), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  // Acima os arrays são fixos e de tamanho conhecido pelo compilador.

  std::vector<int> v3(10); // 10 int consecutivos na memória.
  // O tamanho não precisa ser conhecido pelo compilador e pode
  // ser mudado

  v3.resize(20); // Agora v3 tem 20 int consecutivos.

  std::cout << "v3 vale: ";
  std::copy(begin(v3), end(v3), std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

void tuplas() {
  std::tuple<int, double> t1;
  std::get<0>(t1) = 3;
  std::get<1>(t1) = 2.5;

  std::cout << "t1 = (" << std::get<0>(t1) << ", " << std::get<1>(t1) << ")\n";

  auto t2 = std::make_tuple(4, 3.5);

  std::cout << "t2 = (" << std::get<0>(t2) << ", " << std::get<1>(t2) << ")\n";

  std::tuple<int, double> t3{5, 4.5};

  std::cout << "t3 = (" << std::get<0>(t3) << ", " << std::get<1>(t3) << ")\n";

  auto [i1, d1] = t1; // i1 vale 3, d1 vale 2.5

  std::cout << "i1 = " << i1 << ", d1 = " << d1 << std::endl;

  int i2;
  double d2;

  std::tie(i2, d2) = t2;

  std::cout << "i2 = " << i2 << ", d2 = " << d2 << std::endl;
}

void loops_for() {
  std::vector<int> v(100);

  for (size_t i = 0; i < v.size(); ++i) {
    v[i] = 10 * i;
  }

  for (auto current = begin(v); current != end(v); ++current) {
    *current /= 2;
  }

  for (auto &x : v) {
    x += 1;
  }

  for (auto x : v) {
    std::cout << x << " ";
    // x = 0 (ou outra alteração de valor) seria um erro
  }
  std::cout << std::endl;
}

void parametro_por_valor(int x) {
  // x é uma nova variável local, com valor copiado da chamada
  std::cout << "x comeca com " << x;
  x *= 10; // Variável normal, pode ser alterada
  std::cout << " e termina com " << x << std::endl;
}

void parametro_por_referencia(int &x) {
  // x é referência para o que foi passado na chamada
  std::cout << "x comeca com " << x;
  x *= 10; // Variável normal, pode ser alterada
  std::cout << " e termina com " << x << std::endl;
}

void parametro_por_referencia_constante(int const &x) {
  // x é referência para o que foi passado na chamada
  std::cout << "x comeca com " << x;
  // Não pode alterar. O seguinte é erro
  // x *= 10;
  std::cout << " e termina com " << x << std::endl;
}

void valor_implica_copia(std::vector<int> v) {
  // v é uma cópia do vetor original, que pode ser grande.
  v[0] += v[1]; // Altera cópia, não original.
  std::cout << "dentro de valor_implica_copia, v[0] = " << v[0] << std::endl;
}

void referencia_economiza_copia_e_altera(std::vector<int> &v) {
  // É referência. Usa vetor passado ao invés de fazer cópia.
  v[0] += v[1]; // Altera o vetor passado como parâmetro
  std::cout << "dentro de referencia_economiza_copia_e_altera, v[0] = " << v[0]
            << std::endl;
}

void referencia_constante_impede_alteracao(std::vector<int> const &v) {
  // Recebe referência, então na precisa copiar.
  std::cout << "O primeiro elemento vale " << v[0] << std::endl;
  // Como é const, v não vai ser alterado.
  // v[0] += v[1] seria inválido.
}

int retorno_por_valor() {
  // Retorna um int, cujo valor é dado pelo return
  return 1 << 5;
}

int &retorno_por_referencia(int &a, int &b) {
  // Retorna referência para algo, indicado pelo return
  if (a > b) {
    return a;
  } else {
    return b;
  }
}

std::tuple<int, double> retorno_multiplo() { return {-1, 1.6}; }

void parametros_e_retorno() {
  parametro_por_valor(1);
  int a{5};
  std::cout << "Antes de parametro_por_referencia a = " << a << std::endl;
  parametro_por_referencia(a);
  std::cout << "Depois de parametro_por_referencia a = " << a << std::endl;
  parametro_por_referencia_constante(a);

  std::vector<int> v(1000000);
  v[0] = 1;
  v[1] = 2;
  valor_implica_copia(v); // Caro!
  std::cout << "Depois da volta, v[0] = " << v[0] << std::endl;
  referencia_economiza_copia_e_altera(v);
  std::cout << "Depois da volta, v[0] = " << v[0] << std::endl;
  referencia_constante_impede_alteracao(v);

  a = retorno_por_valor();
  std::cout << "Valor retornado em a: " << a << std::endl;

  int b{7};

  retorno_por_referencia(a, b) = -1;
  std::cout << "a = " << a << ", b = " << b << std::endl;

  auto [c, d] = retorno_multiplo();
  std::cout << "c = " << c << ", d = " << d << std::endl;
}

struct Ponto {
  // Um ponto tem coordenadas x e y
  int x, y;
};

class Contador {
  // Um contador tem um campo int chamado _n, mas ele é privado.
  int _n = 0;

public:
  // Estes são os métodos publicos (usáveis pelo cliente)
  void up() { ++_n; }
  int get() const { return _n; }
};

void struct_e_class() {
  Ponto p{2, 4};
  std::cout << "Ponto em (" << p.x << ", " << p.y << ")\n";
  Contador c;
  c.up();
  c.up();
  std::cout << "Contados " << c.get() << " eventos.\n";
  // c._n seria um erro
}

class Falador {
  int _valor;

public:
  explicit Falador(int a) : _valor(a) {
    std::cout << "Falador com " << _valor << " sendo criado." << std::endl;
  }
  // Abaixo o construtor de cópia
  Falador(Falador const &f) : _valor(f._valor) {
    std::cout << "Um falador com valor " << _valor << " foi copiado.\n";
  }
  int valor() const { return _valor; }
  ~Falador() {
    std::cout << "Falador com " << _valor << " terminando." << std::endl;
  }
};

void falante(Falador f) {
  Falador meu{f.valor() + 10};
  std::cout << "Recebi falador com " << f.valor() << " e criei um com "
            << meu.valor() << std::endl;
}

void escopo_construtores_e_destruidores() {
  std::cout << "Vou criar f1\n";
  Falador f1(1);
  std::cout << "f1 criado\n";
  {
    std::cout << "Vou criar f2\n";
    Falador f2(2);
    std::cout << "f2 criado\nVou rodar falante(f2)\n";
    falante(f2);
    std::cout << "retornou da falante\n";
  }
  std::cout << "Escopo de f2 terminou\nVou chamar falante(f1)\n";
  falante(f1);
  std::cout << "Retornou de falante(f1)\n";
}

class Somador {
  int _valor;

public:
  explicit Somador(int a = 0) : _valor(a) {}
  int valor() const { return _valor; }
  friend Somador operator+(Somador const &a, Somador const &b) {
    return Somador{a._valor + b._valor};
  }
};

std::ostream &operator<<(std::ostream &os, Somador const &s) {
  os << "[" << s.valor() << "]";
  return os;
}

std::istream &operator>>(std::istream &is, Somador &s) {
  int tmp;
  is >> tmp;
  s = Somador{tmp};

  return is;
}

void usa_sobrecarga() {
  Somador s1{1001};
  Somador s2;
  std::cout << "Digite um valor para s2: ";
  std::cin >> s2;

  Somador s3 = s1 + s2;
  std::cout << "s3 vale " << s3 << std::endl;
}

void cresce_vetor() {
  // Começa com um vetor vazio (sem elementos)
  std::vector<Somador> vs;

  // Abaixo o código faz (3 vezes):
  // 1. Cria um novo Somador
  // 2. Aumenta o tamanho vetor de 1 elemento
  // 3. Copia o Somador criado nesse novo elemento
  vs.push_back(Somador{0});
  vs.push_back(Somador{1});
  vs.push_back(Somador{2});

  // O código abaixo faz (3 vezes)
  // 1. Cria um novo elemento no vetor, inicializando com o construtor de
  // Somador recebendo o valor especificado
  vs.emplace_back(3);
  vs.emplace_back(4);
  vs.emplace_back(5);

  std::cout << "Vetor vs contem: ";
  std::copy(begin(vs), end(vs), std::ostream_iterator<Somador>(std::cout, " "));
  std::cout << std::endl;
}

void funcoes_anonimas() {
  // Uma função que não faz nada.
  auto f1 = []() {};
  f1();

  int x{1}, y{2};

  // Uma função sem parâmetro que retorna um valor fixo.
  auto um = []() { return 1; };
  std::cout << "um() retorna " << um() << std::endl;

  // Uma função com um parâmetro
  auto dobro = [](int a) { return 2 * a; };
  std::cout << "dobro(3) retorna " << dobro(3) << std::endl;

  // Uma função que captura o valor de x
  auto mais_x = [x](int a) { return a + x; };
  std::cout << "mais_x(3) retorna " << mais_x(3) << std::endl;
  // E se alteramos x?
  x = 10;
  std::cout << "mais_x(3) retorna " << mais_x(3) << std::endl;

  // Uma função que captura uma referência para x
  auto mais_x_ref = [&x](int a) { return a + x; };
  std::cout << "mais_x_ref(3) retorna " << mais_x_ref(3) << std::endl;
  // E se alteramos x?
  x = 100;
  std::cout << "mais_x_ref(3) retorna " << mais_x_ref(3) << std::endl;

  // Uma função que captura x e y por valor
  auto vezes_y_mais_x = [=](int a) { return a * y + x; };
  std::cout << "vezes_y_mais_x(5) retorna " << vezes_y_mais_x(5) << std::endl;

  // Uma função que captura x e y por referência
  auto vezes_y_mais_x_ref = [&](int a) { return a * y + x; };
  std::cout << "vezes_y_mais_x_ref(5) retorna " << vezes_y_mais_x_ref(5)
            << std::endl;
  // Mudando x e y
  x = -1;
  y = 1000;
  std::cout << "vezes_y_mais_x_ref(5) retorna " << vezes_y_mais_x_ref(5)
            << std::endl;

  std::vector<int> v(1000000);
  v[4] = 1;
  // Podemor também capturar referência constante
  auto i_esimo = [&v = std::as_const(v)](int i) { return v[i]; };
  std::cout << "i_esimo(0) = " << i_esimo(0) << std::endl;
  std::cout << "i_esimo(4) = " << i_esimo(4) << std::endl;
}

int main(int, char const *[]) {
  std::cout << "========================================\n";
  std::cout << "  const e referencias\n";
  const_e_referencias();
  std::cout << "=======================================\n";
  std::cout << "  arranjos lineares\n";
  arranjos_lineares();
  std::cout << "=======================================\n";
  std::cout << "  tuplas\n";
  tuplas();
  std::cout << "=======================================\n";
  std::cout << "  loops for\n";
  loops_for();
  std::cout << "=======================================\n";
  std::cout << "  parametros e retorno\n";
  parametros_e_retorno();
  std::cout << "=======================================\n";
  std::cout << "  struct e class\n";
  struct_e_class();
  std::cout << "=======================================\n";
  std::cout << "  escopo, construtores e destruidores\n";
  escopo_construtores_e_destruidores();
  std::cout << "=======================================\n";
  std::cout << "  sobrecarga de operadores\n";
  usa_sobrecarga();
  std::cout << "=======================================\n";
  std::cout << "  cresce um vetor\n";
  cresce_vetor();
  std::cout << "=======================================\n";
  std::cout << "  funcoes anonimas\n";
  funcoes_anonimas();
  return 0;
}
