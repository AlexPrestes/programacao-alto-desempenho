# Trabalho 1: código sequencial
Você vai desenvolver um código para calcular uma métrica associada com vértices de grafos sem direção. O intuito é desenvolver um código levando em consideração os aspectos de desempenho que estamos estudando, mas ainda sem incluir elementos de paralelismo explícito.

A descrição completa está no PDF anexo.

Para ajudar os testes de correção e desempenho, também é fornecido um pacote de arquivos de teste, com os resultados esperados. Para cada caso, os arquivos com nome da forma xxxxx.edgelist são os arquivos de descrição do grafo, a serem usados como entrada para o seu programa. Os arquivos expected_xxxxx.cls são arquivos com os resultados esperados (dentro de precisão numérica), que você deverá comparar com os arquivos xxxxx.cls gerados pelo seu programa.

Os nomes dos arquivos de teste dão informações sobre o tipo e tamanho do grafo. O tipo não importa para este trabalho (fora que tipos diferentes vão ter características de desempenho diferentes), o tamanho é fornecido em duas partes: Um arquivo *_n_[y]_* onde [y] é um número descreve um grafo com esse número de vértices; já a parte *_k_[z]_* onde [z] é um número indica que esse número é o grau médio dos vértices no grafo. Quanto maior o número de vértices e o grau médio, maior será o tempo de processamento.

Existem dois tipos de arquivos de teste: alguns deles são para grafos bem pequenos, com 10 vértices . Esses arquivos são fornecidos porque eles permitem que vocês desenhem o grafo correspondente e calculem manualmente os valores esperados, e também que vocês comparem manualmente os seus resultados com os esperados.  

Já os arquivos *.edgelist maiores servem dois propósitos: o primeiro é verificação de correção e o segundo avaliação de desempenho. Para usar esses arquivos para verificação de correção, é fornecido o programa compare-expected-trab1.cpp, que você pode compilar e usar para comparar o seu arquivo com o arquivo esperado (veja abaixo).  Para usar em teste de desempenho, você pode verificar o tempo de cálculo do seu programa e como ele depende do tamanho da rede.

Para executar a comparação com o programa fornecido, é necessário que tanto o seu resultado quanto o arquivo com os valores esperados estejam no diretório corrente, que o seu arquivo de resultado tenha o nome conforme o especificado, e então para verificar o seu arquivo xx_yy_zz.cls você deve passar esse nome de arquivo completo (com extensão) mas sem incluir outras informações (como diretório, etc) na linha de comando.
