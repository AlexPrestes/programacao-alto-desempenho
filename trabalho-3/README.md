# Trabalho 3: MPI
O código anexo realiza o seguinte:

1. Gera dois vetores de um tamanho especificado na linha de comando.
2. Para os dois vetores, calcula uma função demorada de cada um dos seus elementos.
3. Em seguinda, encontra qual a maior diferença absoluta entre valores retornados pelas funções dos dois vetores. Isto é, ele calcula max(f(xi)-f(yj)) para todos os valores e i e j, onde xi e yj são os elementos dos vetores gerados.

Seu trabalho é implementar uma versão paralela em MPI desse código, sujeita à limitação de que as gerações dos valores iniciais dos dois vetores devem ser feitas inteiramente no rank 0 (pois essa geração aleatória está apenas simulando a leitura de valores de arquivos).

Você pode reorganizar o código do jeito que achar conveniente, desde que ele realize as mesmas operações.

Entregue o código fonte do seu programa MPI.
