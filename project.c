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


Data data_atual = {0, 0, 0};
Hora hora_atual = {0, 0};

int main () {
    HashTable *hashTable = cria_HashTable();
    processar_input(hashTable);
    free(hashTable);
    return 0;
}

// Função para atualizar a data e hora atuais
void atualizar_tempo(Data *data, Hora *hora, Data nova_data, Hora nova_hora) {
    *data = nova_data;
    *hora = nova_hora;
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
                //processar_saidas(hashTable);
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



/*Retorna TRUE se matricula for uma matricula válida*/
int matricula_valida(char *matricula) {
    int num_counter = 0, let_counter = 0;

    //verifica primeiro par
    if (isupper(matricula[0]) && isupper(matricula[1]))
        let_counter += 1;
    else if (isdigit(matricula[0]) && isdigit(matricula[1]))
        num_counter += 1;

    //verifica segundo par
    if (isupper(matricula[3]) && isupper(matricula[4]))
        let_counter += 1;
    else if (isdigit(matricula[3]) && isdigit(matricula[4]))
        num_counter += 1;

    //verifica terceiro par
    if (isupper(matricula[6]) && isupper(matricula[7]))
        let_counter += 1;
    else if (isdigit(matricula[6]) && isdigit(matricula[7]))
        num_counter += 1;

    //verifica counters
    if (num_counter == 0 || let_counter == 0 || num_counter + let_counter != 3)
        return FALSE;
    else
        return TRUE;
}


/*Retorna o dia final de um mês*/
int diaFinalMes (int m) {
    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) {
        return 31;
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        return 30;
    } else if (m == 2) {
        return 28;
    }

    return 0;
}

/*Retorna TRUE se d for uma data válida*/
int dataValida(Data d) {
    int diasmes = diaFinalMes(d.m);

    if (d.d < 1 || d.d > diasmes || d.m < 1 || d.m > 12) {
        return FALSE;
    }
    return TRUE;
}

/*Retorna TRUE se h for uma hora válida*/
int horaValida(Hora h) {
    if (h.min < 0 || h.min >= 60 || h.h < 0 || h.h >= 24) {
        return FALSE;
    }
    return TRUE;
}

/*Retorna TRUE se d2 for mais recente que d1, FALSE caso contrário*/
int dataRecente(Data d1, Data d2) {
    if (d1.a > d2.a)
        return FALSE;
    if (d1.a < d2.a)
        return TRUE;
    if (d1.m > d2.m)
        return FALSE;
    if (d1.m < d2.m)
        return TRUE;
    if (d1.d > d2.d)
        return FALSE;
    return TRUE;
}

/*Retorna TRUE se h2 for mais recente que h1, FALSE caso contrário*/
int horaRecente(Hora h1, Hora h2) {
    if (h1.h > h2.h)
        return FALSE;
    if (h1.h < h2.h)
        return TRUE;
    if (h1.min > h2.min)
        return FALSE;
    return TRUE;
}

/*Retorna TRUE se o instante 2 é mais recente que o 1, FALSE caso contrário*/
int data_hora_valida_e_recente(Data d1, Hora h1, Data d2, Hora h2) {
    if (dataRecente(d1, d2) && horaRecente(h1, h2) && dataValida(d2) && horaValida(h2))
        return TRUE;
    else
        return FALSE;
}

Data datamaismais(Data d) {
    d.d++;
    int diasmes = diaFinalMes(d.m);

    if(d.d > diasmes){
        d.m++;
        if(d.m == 13){
            return (Data){1, 1, d.a+1};
        }
        else return (Data){1, d.m, d.a};
    }
    return (Data){d.d, d.m, d.a};
}

int calcula_minutos_entre_datas(Data d1, Hora h1, Data d2, Hora h2) {
    int diasinc = 0;
    
    while(!dataRecente(d2, d1)){
        diasinc++;
        d1 = datamaismais(d1);
    }

    return diasinc*24*60 + h2.h*60 + h2.min - h1.h*60 -h1.min;
}



int insere_entrada_parque(Parque *parque, char *matricula, Data data, Hora hora, HashTable *hashTable) {
    
    // Verifica se o parque está cheio
    if (parque->lugares_disponiveis <= 0) {
        return -1;
    }

    // Verifica se a matrícula é válida
    if (!matricula_valida(matricula)) {
        return -2;
    }

    // Verifica se o carro já está dentro de um parque
    nodeHASH *estado_matricula = procura_na_hastable(hashTable, matricula);
    if (estado_matricula != NULL && estado_matricula->estado == DENTRO) {
        return -3;
    }
    
    
    // Verifica se a data e hora são válidas
    if (!data_hora_valida_e_recente(data_atual, hora_atual, data, hora)) {
        return -4;
    }
     

    // Alocar memória para o novo registro
    Registo_entradas *novo_registo = malloc(sizeof(Registo_entradas));
    if (novo_registo == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Preencher os dados do novo registro
    strcpy(novo_registo->matricula, matricula);
    novo_registo->data = data;
    novo_registo->hora = hora;
    novo_registo->estado = LIVRE;
    novo_registo->next = NULL;
    
    // Inserir o novo registro na lista de entradas do parque
    if (parque->entradas == NULL) {
        parque->entradas = novo_registo;
    } else {
        // Percorrer a lista até o último nó
        Registo_entradas *atual = parque->entradas;
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

    atualizar_tempo(&data_atual, &hora_atual, data, hora);

    return 0;
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
        Data data_entrada;
        Hora hora_entrada;

        // Converte a data para o formato desejado
        sscanf(argumentos[2], "%d-%d-%d", &data_entrada.d, &data_entrada.m, &data_entrada.a);
        
        // Converte a hora para o formato desejado
        sscanf(argumentos[3], "%d:%d", &hora_entrada.h, &hora_entrada.min);

        // Verifica se o parque existe
        int indice_parque = parque_existe(nome_parque);
        if (indice_parque) {
            // Obtém o parque
            Parque *parque = &stored_parques[indice_parque - 1];
            
            int resultado = insere_entrada_parque(parque, matricula, data_entrada, hora_entrada, hashTable);
            if (resultado == 0) {
                // Imprime a informação sobre o parque após cada entrada
                printf("%s %d\n", parque->nome_parque, parque->lugares_disponiveis);
            }

            else if (resultado == -1) {
                printf("%s: parking is full.\n", nome_parque);
            }
            
            else if (resultado == -2) {
                printf("%s: invalid licence plate.\n", matricula);
            }

            else if (resultado == -3) {
                printf("%s: invalid vehicle entry.\n", matricula);
            }

            
            else if (resultado == -4) {
                printf("invalid date.\n");
            }
            

        } 
        
        else {
            printf("%s: no such parking.\n", nome_parque);
        }
    }
}

/*
float calcular_custo_estadia(float valor_15, float valor_15_apos_1hora, float valor_max_diario, int min_estadia) {
    const int min_por_dia = 24 * 60;
    const int min_por_periodo = 15;

    //Calcula quantos dias completos passaram
    int dias = min_estadia / min_por_dia;
    
    //periodos são blocos de até 15 minutos (16 minutos contam como 2 periodos)
    int periodos = (min_estadia - (dias * min_por_dia)) / min_por_periodo;
    if ((min_estadia % min_por_periodo) != 0) {
        periodos += 1;
    }

    float custo_total = 0.0;
    custo_total += dias*valor_max_diario + min(valor_max_diario, min(4, periodos) * valor_15 + max(0, periodos - 4) * valor_15_apos_1hora);
    
    return custo_total;
}


//COMANDO s



Registo_entradas* procura_matricula_parque(Parque *parque, char *matricula) {
    // Verifica se o parque é válido
    if (parque == NULL) {
        return NULL;
    }

    // Percorre a lista de registros de entrada do parque
    Registo_entradas *atual = parque->entradas;
    while (atual != NULL) {
        // Compara a matrícula atual com a matrícula procurada
        if (strcmp(atual->matricula, matricula) == 0) {
            // Retorna o registro se a matrícula for encontrada
            return atual;
        }
        // Passa para o próximo registro
        atual = atual->next;
    }

    // Retorna NULL se a matrícula não for encontrada
    return NULL;
}

void atualiza_mat_hashtable_fora(HashTable *hashTable, char *matricula) {
    // Calcula o índice da matrícula na tabela hash
    int index = hash(matricula);

    // Obtém o nó correspondente à matrícula na tabela hash
    nodeHASH *node = hashTable->table[index];

    // Procura a matrícula na lista encadeada
    while (node != NULL) {
        if (strcmp(node->matricula, matricula) == 0) {
            // Atualiza o estado do veículo para FORA
            node->estado = FORA;
            return;
        }
        node = node->next;
    }

    // Se a matrícula não for encontrada, não há nada para atualizar
}


int insere_saida_parque(Parque *parque, char *matricula, Data data, Hora hora, HashTable *hashTable) {

    // Verifica se a matrícula é válida
    if (!matricula_valida(matricula)) {
        return -1;
    }

    // Verifica se o carro está dentro do parque
    nodeHASH *estado_matricula = procura_na_hastable(hashTable, matricula);
    if (estado_matricula != NULL && estado_matricula->estado == FORA) {
        return -2;
    }
    
    // Verifica se a data e hora são válidas
    if (!data_hora_valida_e_recente(data_atual, hora_atual, data, hora)) {
        return -3;
    }


    // Alocar memória para o novo registro
    Registo_saidas *novo_registo = malloc(sizeof(Registo_saidas));
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
    if (parque->saidas == NULL) {
        parque->saidas = novo_registo;
    } else {
        // Percorrer a lista até o último nó
        Registo_saidas *atual = parque->saidas;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        // Adicionar o novo registro no final da lista
        atual->next = novo_registo;
    }

    // Atualizar o estado do veículo para FORA na hashtable de matrículas
    atualiza_mat_hashtable_fora(hashTable, matricula);

    // Aumentar em 1 o número de lugares disponíveis
    parque->lugares_disponiveis++;

    //Atualiza o tempo atual
    data_atual = data;
    hora_atual = hora;

    return 0;
}



void processar_saidas(HashTable *hashTable) {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;

    // Lê a linha de comando
    leLinha(argumentos, &n_argumentos);

    // Verifica se o número de argumentos é válido
    if (n_argumentos == 4) {
        char *nome_parque = argumentos[0];
        char *matricula = argumentos[1];
        Data data_saida;
        Hora hora_saida;

        // Converte a data de saída para o formato desejado
        sscanf(argumentos[3], "%d-%d-%d", &data_saida.d, &data_saida.m, &data_saida.a);
        
        // Converte a hora de saída para o formato desejado
        sscanf(argumentos[4], "%d:%d", &hora_saida.h, &hora_saida.min);

        // Verifica se o parque existe
        int indice_parque = parque_existe(nome_parque);
        if (indice_parque) {
            // Obtém o parque
            Parque *parque = &stored_parques[indice_parque - 1];
            
            
            int resultado = insere_saida_parque(parque, matricula, data_saida, hora_saida, hashTable);
            if (resultado == 0) {
                // Imprime a informação sobre o parque após cada saida.
                //int minutos_estadia = calcula_minutos_entre_datas(data_entrada, hora_entrada, data_saida, hora_saida);
                //float custo_estadia = calcular_custo_estadia(parque->valor_15, parque->valor_15_apos_1hora, parque->valor_max_diario, minutos_estadia);

                //printf("%s %s %s %s %s %.2f\n", parque->nome_parque, data_entrada, hora_entrada, data_saida, hora_saida, custo_estadia);
            }
            
            else if (resultado == -1) {
                printf("%s: invalid licence plate.\n", matricula);
            }

            else if (resultado == -2) {
                printf("%s: invalid vehicle entry.\n", matricula);
            }

            else if (resultado == -3) {
                printf("invalid date.\n");
            }
        }
         else {
            printf("%s: no such parking.\n", nome_parque);
        }
    }
}

*/


//vamos procurar muitas vezes as entradas e saidas dos carros
//começar por onde da speed up e usar tabelas de dispersão

//hastables para entradas e saidas

/*

void liberta_hashtable(HashTable *hashTable) {
    if (hashTable == NULL) {
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        nodeHASH *current = hashTable->table[i];
        while (current != NULL) {
            nodeHASH *temp = current;
            current = current->next;
            free(temp->matricula);
            free(temp);
        }
    }

    free(hashTable);
}


void liberta(Parque stored_parques[MAX_PARQUES], HashTable *hashTable) {
    for (int i = 0; i < MAX_PARQUES; i++) {
        free(stored_parques[i].nome_parque);
        stored_parques[i].nome_parque = NULL;
        destroy_node_entradas(stored_parques[i].)
    }

    liberta_hashtable(hashTable);
}
*/




/* CENAS PARA FAZER

//COMANDO e
- Corrigir atualização da hora/data atual

//COMANDO s
- Quando sai um carro, adicionar no registo de saidas desse parque e alterar o estado do carro no registo de entradas para CONECTADO

//COMANDO v
- Ordenar primeiro a lista de parques pelo nome
- Iterar pelo registo de entradas de cada parque e filtrar pela matrícula
--> Se estado == CONECTADO, então imprime data/hora entrada e data/hora saida
--> Se estado == LIVRE, então imprime só data/hora entrada

//COMANDO f
- Com um argumento: Iterar pelos registos de saida do parque e somar todos os custos associados à mesma data. Imrpimir data e custo nessa data.
- Com dois argumentos: Iterar pelos registos de saida do parque e ir somando o parâmetro custo de todas os nos da data específica.
Erros:
- parque_existe()
- Data inválida: dataRecente() e dataValida()

*/