/*
    Ficheiro: project.h
    Autor: Filipe Oliveira (ist1110633)
    Descrição: Ficheiro com as estruturas de dados e funções utilizadas no 
               ficheiro project.c
*/

#ifndef MAIN_H
#define MAIN_H

/*Bibliotecas importadas*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

/*Máximo de caracteres num input*/
#define BUFSIZ 8192
#define MAX_INPUT BUFSIZ

/*Número máximo de argumentos em qualquer comando*/
#define MAX_ARGUMENTOS 5

/*Número máximo de parques permitidos no sistema*/
#define MAX_PARQUES 20


/*Estrutura Parque*/
typedef struct {
    char nome_parque[BUFSIZ];
    int capacidade;
    int lugares_disponiveis;
    float valor_15;
    float valor_15_apos_1hora;
    float valor_max_diario; 
} Parque;


/*Estrutura Data (DD-MM-AAAA)*/
typedef struct {
    int d, m, a;
} Data;


/*Estrutura Hora (HH:MM)*/
typedef struct {
    int h, min;
} Hora;


/*Estrutura Carro*/
typedef struct {
    char nome_parque[BUFSIZ];
    char matricula;
    Data data;
    Hora hora; 
} Carro;


/*Estrutura da lista ligada*/
typedef struct node{
    int id;
    struct node *next;
} Node;



/*Funções utilizadas*/
void leLinha(char list_of_words[][MAX_INPUT], int *argumentos);

Node *insert_begin(Node *head, int new_id);
Node *insert_end(Node *tail, int new_id);
Node *remove_node(Node *head, Node *node_to_remove);
int length(Node *head);

int parque_existe(char nome_parque[]);
Parque obter_parque_por_id (int id);
int criar_parque(char nome_parque[], int capacidade, float valor_15, float valor_15_apos_1hora, float valor_max_diario);




void processar_input();
void processar_parques();


#endif