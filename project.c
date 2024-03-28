/*
    Ficheiro: project.c
    Autor: Filipe Oliveira (ist1110633)
    Descrição: Gestão de parques de estacionamento (Projeto IAED 2023/24).
*/

#include "project.h"

/*Variável global que armazena o número de parques no sistema.*/
int N_parques = 0;

/*Lista de parques no sistema.*/
Parque stored_parques[MAX_PARQUES];


int main () {
    processar_input();
    return 0;
}


/*Recebe o input introduzido pelo utilizador.
Enquanto não for introduzido o comando "q", o programa continua a correr.*/
void processar_input () {
    int running = TRUE;

    while (running) {
        char input = getchar();

        /*Apenas entra no switch se o input for um dos comandos disponíveis.
        Para cada comando disponível, existe a função correspondente.*/

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
    
}


/*Analisa o input do utilizador, separando-o por palavras (argumentos).
Operacional no caso de haver aspas, espaços ou tabulador horizontal.*/
void leLinha(char list_of_words[][MAX_INPUT], int *argumentos) {
    int i = 0, c, aspas = FALSE, comecar = FALSE;
    
    /* Valor inicial de argumentos*/
    *argumentos = 0;

    while ((c = getchar()) != EOF && c != '\n')
    {
        /* Ignora o primeiro espaco entre o comando e o primeiro argumento*/
        if(!comecar) {
            comecar = TRUE;
            continue;
        }
        
        if ((isspace(c) || c == '\t') && !aspas) {
            /*Ignora espaços em branco extras entre argumentos do input*/
            if (i == 0)
                continue;

            /* Adiciona no argumento atual o caracter terminal*/
            list_of_words[*argumentos][i] = '\0';
            (*argumentos)++;
            i = 0;
        }
        else {
            /* Verifica se estamos dentro de um par de aspas*/
            if (c == '"') {
                aspas = !aspas;
                continue;
            }
            /* Adiciona o caractere atual à palavra atual*/
            list_of_words[*argumentos][i++] = c;
        }
    }
    /* Adiciona à última palavra o caracter terminal*/
    if (i > 0) {
        list_of_words[*argumentos][i] = '\0';
        (*argumentos)++;
    }
}


/*Insere um elemento no início de uma lista ligada.*/
Node *insert_begin(Node *head, int new_id) {
    Node *novo = malloc(sizeof(Node));
    novo->id = new_id;

    novo->next = head;
    head = novo;
    return head;
}


/*Insere um elemento no final de uma lista ligada.*/
Node *insert_end(Node *tail, int new_id) {
    Node *novo = malloc(sizeof(Node));
    novo->id = new_id;

    tail->next = novo;
    novo->next = NULL;
    tail = novo;
    return tail;
}


/*Remove um elemento de uma lista ligada*/
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


/*Calcula o número de elementos de uma lista ligada.*/
int length(Node *head) {
    int count = 0;
    Node *x = head;
    while (x != NULL) {
        count++;
        x = x->next;
    }
    return count;
}



/*Verifica se um determinado parque já existe no sistema.
Se existir retorna o índice do parque, caso contrário retorna 0.*/
int parque_existe(char nome_parque[]) {
    int i;
   
   /*Percorre todas os parques, comparando o nome de cada um com o nome dado*/
    for (i = 0; i < N_parques; i++) {
        if (strcmp(stored_parques[i].nome_parque, nome_parque) == 0) {
            return i + 1;
        }
    }
    return FALSE;
}



/*Obtém o parque de um determinado índice*/
Parque obter_parque_por_id (int id) {
    return stored_parques[id - 1];
}


/*Cria um parque dentro da lista stores_parques.*/
int criar_parque(char nome_parque[], int capacidade, float valor_15,
                 float valor_15_apos_1hora, float valor_max_diario) {
    
    Parque novo_parque;

    /*Recorre à função parque_existe para verificar se o parque já existe*/
    if (parque_existe(nome_parque)) {
        printf ("%s: parking already exists.\n", nome_parque);
        return FALSE;
    }

    /*O parque não pode ter capacidade nula ou negativa*/
    if (capacidade <= 0) {
        printf("%d: invalid capacity.\n", capacidade);
        return FALSE;
    }

    /*Regras dos valores do sistema de faturação*/
    if (!((valor_15 < valor_15_apos_1hora) &&
        (valor_15_apos_1hora < valor_max_diario)) || valor_15 <= 0 ||
        valor_15_apos_1hora <= 0 || valor_max_diario <= 0) {
        
        printf("invalid cost.\n");
        return FALSE;
    }

    /*Verifica se o limite máximo de parques no sistema foi atingido*/
    if (N_parques >= MAX_PARQUES) {
        printf("too many parks.\n");
        return FALSE;
    }

    /*Atribui os valores introduzidos pelo utilizador na estrutura do parque*/
    strcpy(novo_parque.nome_parque, nome_parque);
    novo_parque.capacidade = capacidade;
    novo_parque.valor_15 = valor_15;
    novo_parque.valor_15_apos_1hora = valor_15_apos_1hora;
    novo_parque.valor_max_diario = valor_max_diario;

    /*Adiciona o novo parque à lista de parques no sistema.*/
    stored_parques[N_parques++] = novo_parque;

    return TRUE;
}


/*Processa o input do comando "p" dado pelo utilizador.*/
void processar_parques() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos, i;

    leLinha(argumentos, &n_argumentos);

    /*Se não houver argumentos, lista os parques por ordem de criação*/
    if (n_argumentos == 0) {
        for (i = 0; i < N_parques; i++) {
            printf("%s %d %d\n", stored_parques[i].nome_parque, 
            stored_parques[i].capacidade, stored_parques[i].capacidade);
        }
        return;
    } 
    
    /*Se forem dados argumentos, cria um parque se possível, com o input dado*/
    else if (n_argumentos == 5) {
        criar_parque(argumentos[0], atof(argumentos[1]), atof(argumentos[2]),
                     atof(argumentos[3]), atof(argumentos[4]));
    }
}






//vamos procurar muitas vezes as entradas e saidas dos carros
//começar por onde da speed up e usar tabelas de dispersão

//hastables para entradas e saidas