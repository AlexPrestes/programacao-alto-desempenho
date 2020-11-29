#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;

enum Error { SUCCESS, BAD_ARGUMENT };

// A estrutura escolhida para armazenar o grafo
// foi a lista de adjacencia, pela facilidade de
// gerar a lista e de encontrar os vizinhos.
// como maior vantagem, a maior capacidade de
// armazenamento de grafo.
// desvantagem, o alto custo computacional para
// verificar os clusters/triangulos.
//
// Abaixo a implementação de uma classe lista de adjacencia
// o construtor precisa receber a quatidade de nós.
class AdjacencyList {
  vector< vector<size_t> > _data;
  size_t _sizeVertex;

public:
  AdjacencyList(size_t &sizeVertex)
      : _data(sizeVertex), _sizeVertex(sizeVertex) {}
 
  // insere um novo vertice, a partir do par de arestas
  void insert_edge(pair<int,int> edge) {
    _data[edge.first].push_back(edge.second);
    _data[edge.second].push_back(edge.first);
  }

  vector<size_t> get_edge(size_t &i_vertex) {return _data[i_vertex]; }
  
  size_t get_size_vertex() { return _sizeVertex; }
  
  size_t get_size_edge(size_t &i_vertex) { return _data[i_vertex].size(); }
 
  // função que retorna a quatidade de triangulos por vertice
  // o código faz uma busca simples correndo pares (j, k) de vertices vizinhos do i
  // depois escolhe o vertice vizinhos com menor numero de nós adjacentes, e corre
  // esses vizinhos até encontrar o cluster, por fim sai do loop.
  float get_size_triangle(size_t &i_vertex) {
    auto size_triangle = 0;


    for (auto j_vertex = _data[i_vertex].begin(); j_vertex != _data[i_vertex].end(); ++j_vertex) {
      auto j_size = _data[*j_vertex].size();

      for (auto k_vertex = j_vertex + 1; k_vertex != _data[i_vertex].end(); ++k_vertex ) {
        auto k_size = _data[*k_vertex].size();

        if (k_size < j_size) {
          swap(*j_vertex, *k_vertex);
        }

        for (auto kj_vertex = _data[*j_vertex].begin(); kj_vertex != _data[*j_vertex].end(); ++kj_vertex) {
            if (*kj_vertex == *k_vertex) {
              size_triangle += 1;
              break;
            }
          }

        if (k_size < j_size) {
          swap(*j_vertex, *k_vertex);
        }

        }
      }

    return size_triangle;
  }
};

//
// Prototipos
//
// Função que le o arquivo e armazena na lista de adjacencia
void populaGraph(AdjacencyList &graphAL, string &filename);
// função que calcula o coeficiente de aglomeração
float agglomeration(float &triangle, float &edge);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << argv[0] << " <filename.edgelist> <num_threads>" << endl;
    exit(BAD_ARGUMENT);
  }
 
  float size_triangle, size_edge;
  string filename = argv[1];
  size_t pos = 5;
  size_t pos1 = filename.find('_', pos);
  size_t N = stoul(filename.substr(pos, pos1-pos));
  vector<float> coefAgglo(N);
  int num_threads = stoi(argv[2]);

  chrono::high_resolution_clock::time_point t0;
  chrono::high_resolution_clock::time_point t1;

  AdjacencyList graphAL(N);

  populaGraph(graphAL, filename);

  t0 = chrono::high_resolution_clock::now();
#pragma omp parallel for default(none) private(size_edge, size_triangle) \
            shared(N, graphAL, coefAgglo) num_threads(num_threads) schedule(dynamic)
  // loop que faz a chamada para cada nó do grafo
  // e armazena a saida em um vetor
  for (size_t i = 0; i < N; i++) {
    size_edge = graphAL.get_size_edge(i);
    size_triangle = graphAL.get_size_triangle(i);
    coefAgglo[i] = agglomeration(size_triangle, size_edge);
  }
  
  t1 = chrono::high_resolution_clock::now();
  cout << (t1-t0).count()/1e9 << " (s)" << endl;
 
  // abre o arquivo de saida e grava dos dados
  ofstream ofilename;
  size_t pos_dot = filename.find('.');
  ofilename.open(filename.substr(0, pos_dot) + ".cls");
  for (auto c = coefAgglo.begin(); c != coefAgglo.end(); ++c) {
    ofilename << *c << endl;
  }
  ofilename.close();
  
  // THE END
  return 0;
}

float agglomeration(float &triangle, float &edge) { return (edge < 2) ? 0 : 2*triangle/(edge*(edge-1)); }

void populaGraph(AdjacencyList &graphAL, string &filename) {
  string line;
  int node1, node2;
  
  // Abertura do arquivo
  ifstream datafile(filename);
  if (datafile.fail()) {
    cerr << "Erro na leitura do arquivo " << filename << endl;
    exit(BAD_ARGUMENT);
  }
  // Leitura dos dados do Arquivo
  datafile >> node1;
  datafile >> node2;
  
  while (!datafile.eof()) {
    
    graphAL.insert_edge(pair{node1,node2});
    
    datafile >> node1;
    datafile >> node2;
  }

  datafile.close();

  return;
}
