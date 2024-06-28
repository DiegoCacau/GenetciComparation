
## Requisitos

Para rodar os testes é necessário: 
- possuir um programa para autômato celular
- possuir séries de dados de infectados ou mortos, para determinado período de tempo
- ajustar dados no arquivo `genetic.cpp` que estão hardcoded

A ideia desse código é rodar um script `sh`, no caso usamos o `run_genetic_comparation.sh` e analisar os dados de autput, classificando os cromossomos de acordo com a função de fitness.


## Passos

1 - Na linha 207, atulize a string para apontar para o arquivo com a série temporal que será usada de base. É importante que seja um `csv` de apenas uma coluna com os dados a serem testados (infectados ou mortos)

2 - Na linha 114 está apontando para o arquivo de output do autômato celular e precisa ser alterada também. Importante notar que esse arquivo deverá ter a mesma forma do arquivo do ponto 1.

3- Na linha 107 está apontando para o script `sh` que vai receber os parâmetros do cromossomo e vai rodar o autômato celular. É preciso alterar esse valor também

4 - Na linha 57 existe uma proteção para impedir que sejam analisados arquivos gerados pelo autômato menores do que o limite estipulado. Esse valor precisa ser alterado de acordo com a necessidade dos testes a serem executados

5 - Das linhas 12 a 16 existem os parâmetros de configuração do algoritmo genético e precisam ser alterados de acordo com as necessidade dos testes, tendo o controle do tamanho da população, número de gerações, taxa de mutação, quantidade de parentes da próxima geração e quantidade de cromossomos que sofrerão mutação

## Resultado

O resultado será dado na forma de output pelo terminal, da sequinte forma:

```
Melhor solução encontrada:
Parâmetro 1: 0.001
Parâmetro 2: 1.02113
Parâmetro 3: 0.0852338
Fitness: 314.667
```

## Compilação e execução

Para compilar esse código, basta rodar no terminal:

```
g++ -o genetic genetic.cpp 
```

Para executar, basta rodar:

```
./genetic
```