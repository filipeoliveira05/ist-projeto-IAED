# IAED 2023/24 - Projeto
**Filipe Oliveira**  
**Número de estudante**: ist1110633

## Descrição do Projeto
Este repositório contém o projeto de IAED 2023/24. O projeto consiste num sistema de gestão de parques de estacionamento, permitindo a definição de parques, o registo de entradas e saídas de veículos, assim como a sua consulta e faturação.

## Funcionalidades
O sistema de gestão de parques de estacionamento permite:

- Definição de parques de estacionamento com um regime de faturação.
- Registo de entradas e saídas de veículos nos parques.
- Consulta das entradas e saídas de veículos.
- Consulta da faturação dos parques de estacionamento.

## Compilação e Execução

Para compilar o projeto, utilizar o seguinte comando:

```bash
gcc -Wall -Wextra -Werror -Wno-unused-result
```


## Estrutura do Repositório

- [enunciado.md](./enunciado.md): Contém a especificação completa do projeto, incluindo os comandos suportados e os requisitos funcionais.
- [guidelines.md](./guidelines.md): Contém as diretrizes de formatação, comentários, organização do código e uso de constantes.
- [debugging.md](./debugging.md): Contém informações detalhadas sobre como depurar o projeto, incluindo o uso de ferramentas como valgrind e fsanitize.

## Depuração

Para depurar o projeto, consulte o [debugging.md]. Algumas das ferramentas recomendadas incluem:

- `valgrind`: Para analisar o uso de memória.
- `fsanitize`: Para detectar erros de memória.
- `cppcheck`: Para verificar possíveis fontes de erro no código.

## Exemplos de Comandos

### Comando `p`

Cria um parque de estacionamento ou lista os parques existentes.

### Comando `e`

Regista a entrada de um veículo num parque.

### Comando `s`

Regista a saída de um veículo de um parque.

### Comando `v`

Lista as entradas e saídas de um veículo.

### Comando `f`

Mostra a faturação de um parque de estacionamento.

### Comando `r`

Remove um parque de estacionamento do sistema.