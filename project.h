#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define BUFSIZ 8192
#define MAX_INPUT BUFSIZ

#define MAX_ARGUMENTOS 5
#define MAX_PARQUES 20


typedef struct {
    char nome_parque[BUFSIZ];
    int capacidade;
    int lugares_disponiveis;
    float valor_15;
    float valor_15_apos_1hora;
    float valor_max_diario; 
} Parque;


typedef struct {
    int d, m, a;
} Data;


typedef struct {
    int h, min;
} Hora;


typedef struct {
    char nome_parque[BUFSIZ];
    char matricula;
    Data data;
    Hora hora; 
} Carro;


typedef struct node{
    int id;
    struct node *next;
} Node;




Node *insert_begin(Node *head, int new_id);
Node *insert_end(Node *tail, int new_id);
Node *remove_node(Node *head, Node *node_to_remove);
int length(Node *head);
void print(Node *head);
Node *lookup(Node *head, char nome_parque);


void processar_input();
void processar_parques();
void leLinha(char list_of_words[][MAX_INPUT], int *argumentos);