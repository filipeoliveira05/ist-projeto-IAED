/* iaed24 - ist1110633 - project */

#include "project.h"

int N_parques = 0;

Parque stored_parques[MAX_PARQUES];

int main () {
    processar_input();
    return 0;
}


void processar_input () {
    int running = TRUE;

    while (running) {
        char input = getchar();

        switch (input) {
            case 'q':
                running = FALSE;
                //free();
                break;
            case 'p':
                processar_parques();
                break;
            case 'e':
                //código
                break;
            case 's':
                //código
                break;
            case 'f':
                //código
                break;
            case 'v':
                //código
                break;
            case 'r':
                //código
                break;
        }

    }
    
    //free();
}



void leLinha(char list_of_words[][MAX_INPUT], int *argumentos) {
    int i = 0, c, aspas = FALSE, comecar = FALSE;
    /* Define o valor inicial para o número de argumentos */
    *argumentos = 0;
    while ((c = getchar()) != EOF && c != '\n')
    {
        /* Ignora o primeiro espaco entre o comando e o primeiro argumento */
        if(!comecar) {
            comecar = TRUE;
            continue;
        }
        
        if ((isspace(c) || c == '\t') && !aspas) {
            /* 
                Ignora espaços em branco extras entre argumentos do
                comando (quando nao e um nome entre aspas)
            */
            if (i == 0)
                continue;

            /* Adiciona no argumento atual o caracter terminal */
            list_of_words[*argumentos][i] = '\0';
            (*argumentos)++;
            i = 0;
        }
        else {
            /* Verifica se estamos dentro de um par de aspas */
            if (c == '"') {
                aspas = !aspas;
                continue;
            }
            /* Adiciona o caractere atual à palavra atual */
            list_of_words[*argumentos][i++] = c;
        }
    }
    /* Adiciona a última palavra o caracter terminal */
    if (i > 0) {
        list_of_words[*argumentos][i] = '\0';
        (*argumentos)++;
    }
}



Node *insert_begin(Node *head, Parque *parque) {
    Node *x = malloc(sizeof(Node));
    x->parque = parque;
    x->next = head;
    return x;
}

Node *insert_end(Node *head, Parque *parque) {
    Node *x = malloc(sizeof(Node));
    x->parque = parque;
    x->next = NULL;
    if (head == NULL) {
        return x;
    }
    Node *tail;
    for (tail = head; tail->next != NULL; tail = tail->next);
    tail->next = x;
    return head;
}

Node *remove_node(Node *head, Node *node_to_remove) {
    if (head == node_to_remove) {
        Node *new_head = head->next;
        free(head);
        return new_head;
    }
    Node *prev = head;
    Node *curr = head->next;
    while (curr != NULL && curr != node_to_remove) {
        prev = curr;
        curr = curr->next;
    }
    if (curr != NULL) {
        prev->next = curr->next;
        free(curr);
    }
    return head;
}

int length(Node *head) {
    int count = 0;
    Node *x = head;
    while (x != NULL) {
        count++;
        x = x->next;
    }
    return count;
}

void print(Node *head) {
    Node *x = head;
    while (x != NULL) {
        printf("%c %d %.2f %.2f %.2f\n",
               x->parque->nome_parque,
               x->parque->capacidade,
               x->parque->valor_15,
               x->parque->valor_15_apos_1hora,
               x->parque->valor_max_diario);
        x = x->next;
    }
}

/*
Node *lookup(Node *head, char nome_parque) {
    Node *x;
    for (x = head; x != NULL; x = x->next) {
        if (x->parque->nome_parque == nome_parque)
            return x;
    }
    return NULL;
}
*/

void processar_parques() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;
    //Node *head = NULL;

    leLinha(argumentos, &n_argumentos);

    for (int i = 0; i < n_argumentos; i++) {
        
        printf("Argumento %d: %s\n", i+1, argumentos[i]);
    }
}




//somar 1 dia À primeira até chegar à data da segunda, quando


//vamos procurar muitas vezes as entradas e saidas dos carros
//começar por onde da speed up e usar tabelas de dispersão

//hastables para entradas e saidas