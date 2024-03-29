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

/*Tempo atual*/
Data data_atual;
Hora hora_atual;


int main () {
    HashTable *hashTable = cria_HashTable();
    processar_input(hashTable);
    free(hashTable);
    return 0;
}


/*Recebe o input introduzido pelo utilizador.
Enquanto não for introduzido o comando "q", o programa continua a correr.*/
void processar_input (HashTable *hashTable) {
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
                processar_entradas(hashTable);
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
    novo_parque.lugares_disponiveis = capacidade;
    novo_parque.valor_15 = valor_15;
    novo_parque.valor_15_apos_1hora = valor_15_apos_1hora;
    novo_parque.valor_max_diario = valor_max_diario;

    novo_parque.entradas = NULL;
    novo_parque.saidas = NULL;

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
            stored_parques[i].capacidade, stored_parques[i].lugares_disponiveis);
        }
        return;
    } 
    
    /*Se forem dados argumentos, cria um parque se possível, com o input dado*/
    else if (n_argumentos == 5) {
        criar_parque(argumentos[0], atof(argumentos[1]), atof(argumentos[2]),
                     atof(argumentos[3]), atof(argumentos[4]));
    }
}





/*HASHTABLE DE MATRÍCULAS*/

nodeHASH *cria_node_matricula(char *matricula) {
    nodeHASH *newNode = (nodeHASH *)malloc(sizeof(nodeHASH));
    if (newNode == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    newNode->estado = DENTRO;
    newNode->matricula = strdup(matricula);
    newNode->next = NULL;
    return newNode;
}


HashTable *cria_HashTable() {
    HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }

    return hashTable;
}


unsigned long hash(char *matricula) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *matricula++)) {
        hash = ((hash < 5) + hash) + c; /*hash * 33 + c*/
    }

    return hash % TABLE_SIZE;
}



void insere_mat_hashtable(HashTable *hashTable, char *matricula) {
    int index = hash(matricula);
    nodeHASH *newNode = cria_node_matricula(matricula);

    if (hashTable->table[index] == NULL) {
        hashTable->table[index] = newNode;
    }

    else {
        nodeHASH *current = hashTable->table[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}



nodeHASH *procura_na_hastable(HashTable *hashTable, char *matricula) {
    int index = hash(matricula);
    nodeHASH *current = hashTable->table[index];

    while (current != NULL) {
        if (strcmp(current->matricula, matricula) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


void print_hashtable(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("[%d] -> ", i);
        nodeHASH *current = hashTable->table[i];

        while (current != NULL) {
            printf("%s -> ", current->matricula);
            current = current->next;
        }

        printf("NULL\n");
    }
}


void insere_entrada_parque(Parque *parque, char *matricula, Data data, Hora hora, HashTable *hashTable) {
    // Alocar memória para o novo registro
    Registo *novo_registo = malloc(sizeof(Registo));
    if (novo_registo == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Preencher os dados do novo registro
    strcpy(novo_registo->matricula, matricula);
    novo_registo->data = data;
    novo_registo->hora = hora;
    novo_registo->next = NULL;

    // Inserir o novo registro na lista de entradas do parque
    if (parque->entradas == NULL) {
        parque->entradas = novo_registo;
    } else {
        // Percorrer a lista até o último nó
        Registo *atual = parque->entradas;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        // Adicionar o novo registro no final da lista
        atual->next = novo_registo;
    }

    // Atualizar o estado do veículo para DENTRO na hashtable de matrículas
    insere_mat_hashtable(hashTable, matricula);

    // Subtrair 1 do número de lugares disponíveis
    parque->lugares_disponiveis--;
}


void processar_entradas(HashTable *hashTable) {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;

    // Lê a linha de comando
    leLinha(argumentos, &n_argumentos);

    // Verifica se o número de argumentos é válido
    if (n_argumentos == 4) {
        char *nome_parque = argumentos[0];
        char *matricula = argumentos[1];
        Data data;
        Hora hora;

        // Converte a data para o formato desejado
        sscanf(argumentos[2], "%d-%d-%d", &data.d, &data.m, &data.a);
        
        // Converte a hora para o formato desejado
        sscanf(argumentos[3], "%d:%d", &hora.h, &hora.min);

        // Verifica se o parque existe
        int indice_parque = parque_existe(nome_parque);
        if (indice_parque) {
            // Obtém o parque
            Parque *parque = &stored_parques[indice_parque - 1];
            
            // Insere a entrada no parque
            insere_entrada_parque(parque, matricula, data, hora, hashTable);
            // Imprime a informação sobre o parque após cada entrada
            printf("%s %d\n", parque->nome_parque, parque->lugares_disponiveis);
        } else {
            printf("%s: no such parking.\n", nome_parque);
        }
    }
}









//vamos procurar muitas vezes as entradas e saidas dos carros
//começar por onde da speed up e usar tabelas de dispersão

//hastables para entradas e saidas


/*
//CÓDIGO PARA O COMANDO e

int carro_existe(char matricula) {

}


int criar_carro(char nome_parque[BUFSIZ], char matricula, char data,char hora){
    Carro novo_carro;

}

void leData(char *data) {
    Data nova_data;

    sscanf(data, "%d-%d-%d", &nova_data.d, &nova_data.m, &nova_data.a);
}

void processar_entradas() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos, i;

    leLinha(argumentos, &n_argumentos);

    criar_carro(argumentos[0], argumentos[1], argumentos[2], argumentos[3]);
}
*/