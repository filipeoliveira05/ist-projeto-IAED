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



Node *insert_begin(Node *head, int new_id) {
    Node *novo = malloc(sizeof(Node));
    novo->id = new_id;

    novo->next = head;
    head = novo;
    return head;
}

Node *insert_end(Node *tail, int new_id) {
    Node *novo = malloc(sizeof(Node));
    novo->id = new_id;

    tail->next = novo;
    novo->next = NULL;
    tail = novo;
    return tail;
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

/*
void print(Node *head) {
    Node *x = head;
    while (x != NULL) {
        printf("%s %d %.2f %.2f %.2f\n",
               x->parque->nome_parque,
               x->parque->capacidade,
               x->parque->valor_15,
               x->parque->valor_15_apos_1hora,
               x->parque->valor_max_diario);
        x = x->next;
    }
}
*/

/*
Node *lookup(Node *head, char nome_parque) {
    Node *x;
    for (x = head; x != NULL; x = x->next) {
        if (strcmp(x->parque->nome_parque, nome_parque) == 0)
            return x;
    }
    return NULL;
}
*/


//Fazer função que percorre a lista ligada toda e verifica se o parque já existe
//Na função do criar_parque, encontrar forma de conectar o parque criado à lista ligada de parques
//Adicionar condição do número máximo de parques à função criar_parque



int parque_existe(char nome_parque[]) {
    int i;
   
    for (i = 0; i < N_parques; i++) {
        if (strcmp(stored_parques[i].nome_parque, nome_parque) == 0) {
            return i + 1;
        }
    }
    return FALSE;
}

Parque obter_parque_por_id (int id) {
    return stored_parques[id - 1];
}

int criar_parque(char nome_parque[], int capacidade, float valor_15, float valor_15_apos_1hora, float valor_max_diario) {
    Parque novo_parque;

    if (parque_existe(nome_parque)) {
        printf ("%s: parking already exists.\n", nome_parque);
        return FALSE;
    }

    if (capacidade <= 0) {
        printf("%d: invalid capacity.\n", capacidade);
        return FALSE;
    }

    if (!((valor_15 < valor_15_apos_1hora) && (valor_15_apos_1hora < valor_max_diario)) || valor_15 <= 0 || valor_15_apos_1hora <= 0 || valor_max_diario <= 0) {
        printf("invalid cost.\n");
        return FALSE;
    }

    if (N_parques > MAX_PARQUES) {
        printf("too many parks.\n");
        return FALSE;
    }

    strcpy(novo_parque.nome_parque, nome_parque);
    novo_parque.capacidade = capacidade;
    novo_parque.valor_15 = valor_15;
    novo_parque.valor_15_apos_1hora = valor_15_apos_1hora;
    novo_parque.valor_max_diario = valor_max_diario;

    stored_parques[N_parques++] = novo_parque;

    return TRUE;
}


void processar_parques() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos, i;
    //Node *head = NULL;

    leLinha(argumentos, &n_argumentos);

    if (n_argumentos == 0) {
        for (i = 0; i < N_parques; i++) {
            printf("%s %d %d\n", stored_parques[i].nome_parque, 
            stored_parques[i].capacidade, stored_parques[i].capacidade);
        }
        return;
    } else if (n_argumentos == 5) {
        criar_parque(argumentos[0], atof(argumentos[1]), atof(argumentos[2]), atof(argumentos[3]), atof(argumentos[4]));
    }
}




//somar 1 dia À primeira até chegar à data da segunda, quando


//vamos procurar muitas vezes as entradas e saidas dos carros
//começar por onde da speed up e usar tabelas de dispersão

//hastables para entradas e saidas