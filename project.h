/**
 *Descrição: Ficheiro com as estruturas de dados e funções utilizadas no 
             ficheiro project.c
 * @file project.h
 * @author ist1110633 Filipe Oliveira
*/

#ifndef MAIN_H
#define MAIN_H

/*Bibliotecas importadas*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*Funções mínimo e máximo entre dois números*/
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/*Alguns estados possíveis durante o programa (explicados no project.c)*/
#define TRUE 1
#define FALSE 0
#define DENTRO 1
#define FORA 0
#define CONECTADO 1
#define LIVRE 0

/*Máximo de caracteres num input*/
#define BUFSIZ 8192
#define MAX_INPUT BUFSIZ

/*Tamanho hash table*/
#define TABLE_SIZE 5003

/*Número máximo de argumentos em qualquer comando*/
#define MAX_ARGUMENTOS 5

/*Número máximo de parques permitidos no sistema*/
#define MAX_PARQUES 20


/*Estrutura Data (DD-MM-AAAA)*/
typedef struct {
    int d, m, a;
} Data;


/*Estrutura Hora (HH:MM)*/
typedef struct {
    int h, min;
} Hora;


/*Estrutura lista de registos de entrada de um parque*/
typedef struct registo_e {
    char matricula[MAX_INPUT];
    Data data;
    Hora hora;
    Data data_saida;
    Hora hora_saida;
    int estado;
    struct registo_e *next;
} Registo_entradas;


/*Estrutura lista de registos de saídas de um parque*/
typedef struct registo_s {
    char matricula[MAX_INPUT];
    Data data;
    Hora hora;
    float custo;
    struct registo_s *next;
} Registo_saidas;


/*Estrutura Parque*/
typedef struct {
    char nome_parque[BUFSIZ];
    int capacidade;
    int lugares_disponiveis;
    float valor_15;
    float valor_15_apos_1hora;
    float valor_max_diario;
    Registo_entradas *entradas;
    Registo_saidas *saidas;
} Parque;


/*Estrutura da Hash Table e os seus nodes*/
typedef struct nodeHashTable {
    int estado;
    char *matricula;
    struct nodeHashTable *next;
} nodeHASH;

typedef struct HashTable {
    nodeHASH *table[TABLE_SIZE];
} HashTable;


/*Funções utilizadas*/

void atualizar_tempo(Data *data, Hora *hora, Data nova_data, Hora nova_hora);
void le_input(char list_of_words[][MAX_INPUT], int *argumentos);

HashTable *cria_HashTable();
nodeHASH *cria_node_matricula(char *matricula);
unsigned long hash(char *matricula);
void insere_mat_hashtable(HashTable *hashTable, char *matricula);
nodeHASH *procura_mat_na_hastable(HashTable *hashTable, char *matricula);

int parque_existe(char nome_parque[]);
int criar_parque(char nome_parque[], int capacidade, float valor_15,
                 float valor_15_apos_1hora, float valor_max_diario);


int matricula_valida(char *matricula);
int diaFinalMes (int m);
int dataValida(Data d);
int horaValida(Hora h);
int dataRecente(Data d1, Data d2);
int horaRecente(Hora h1, Hora h2);
int dataIgual(Data d1, Data d2);
int horaIgual(Hora h1, Hora h2);
int data_hora_valida_e_recente(Data d1, Hora h1, Data d2, Hora h2);
Data datamaismais(Data d);
int calcula_minutos_entre_datas(Data d1, Hora h1, Data d2, Hora h2);
float calcular_custo_estadia(float valor_15, float valor_15_apos_1hora,
                            float valor_max_diario, int min_estadia);


int insere_entrada_parque(Parque *parque, char *matricula, Data data, Hora hora,
                          HashTable *hashTable);



Registo_entradas* procura_mat_parque_recente(Parque *parque, char *matricula);
Registo_entradas* altera_mat_registo_infosaida(Parque *parque, char *matricula,
                                               Data data_saida,Hora hora_saida);

void atualiza_mat_hashtable_estado_dentro(HashTable *hashTable,char *matricula);
void atualiza_mat_hashtable_estado_fora(HashTable *hashTable, char *matricula);

void atualizar_custo_registo_saida(Parque *parque, char *matricula, Data data,
                                    Hora hora, float novo_custo);


void processar_input (HashTable *hashTable);
void processar_parques();
void processar_entradas(HashTable *hashTable);
void processar_saidas(HashTable *hashTable);
void processar_comando_v();
void processar_faturacao();
void processar_comando_r(HashTable *hashTable);


void mostrar_faturacao_determinado_dia(char *nome_parque, Data data_pesquisa);
void mostrar_faturacao_diaria(char *nome_parque);

void mostrar_registos_veiculo(char *matricula);

void merge(int arr[], int l, int m, int r, Parque stored_parques[MAX_PARQUES]);
void mergeSort(int arr[], int l, int r, Parque stored_parques[MAX_PARQUES]);


void remove_registos_entradas_parque(Parque *parque, HashTable *hashTable);
void remove_registos_saidas_parque(Parque *parque);
void remove_parque(int indice);
void remove_parque_e_registos(char *nome_parque, HashTable *hashTable);


void liberta_hashtable(HashTable *hashTable);
void liberta_registos_entradas_parque(Registo_entradas *lista_registros);
void liberta_registos_saidas_parque(Registo_saidas *registro_saida);
void liberta_todos_os_parques();

#endif