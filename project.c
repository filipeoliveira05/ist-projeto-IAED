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

/*Variáveis globais data e hora atual (atualizado nas entradas/saidas)*/
Data data_atual = {0, 0, 0};
Hora hora_atual = {0, 0};

int main () {
    HashTable *hashTable = cria_HashTable();
    processar_input(hashTable);
    liberta_hashtable(hashTable);
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
                processar_saidas(hashTable);
                break;
            case 'v':
                processar_comando_v();
                break;
            case 'f':
                processar_faturacao();
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
Se existir retorna o índice do parque, cc retorna 0.*/
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





/*Cria um node da hashTable de acordo com a estrutura nodeHASH*/
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


/*Cria uma hashTable de tamanho TABLE_SIZE*/
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

/*Calcula o hash de uma matrícula*/
unsigned long hash(char *matricula) {
    unsigned long hash = 5381;
    int c;
    
    while ((c = *matricula++)) {
        hash = ((hash < 5) + hash) + c;
    }

    return hash % TABLE_SIZE;
}


/*Insere uma matrícula dentro da hashTable*/
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


/*Procura uma determinada matrícula dentro da hashTable*/
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

/*Retorna TRUE se d2 for mais recente ou igual a d1, FALSE cc*/
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

/*Retorna TRUE se h2 for mais recente ou igual a h1, FALSE cc*/
int horaRecente(Hora h1, Hora h2) {
    if (h1.h > h2.h)
        return FALSE;
    if (h1.h < h2.h)
        return TRUE;
    if (h1.min > h2.min)
        return FALSE;
    return TRUE;
}

/* Retorna TRUE se d1 é igual a d2, FALSE cc */
int dataIgual(Data d1, Data d2) {
    if (d1.a == d2.a && d1.m == d2.m && d1.d == d2.d)
        return TRUE;
    return FALSE;
}

/* Retorna TRUE se h1 é igual a h2, FALSE cc */
int horaIgual(Hora h1, Hora h2) {
    if (h1.h == h2.h && h1.min == h2.min)
        return TRUE;
    return FALSE;
}

/* Retorna TRUE se o instante 2 é mais recente ou igual ao 1, FALSE cc */
int data_hora_valida_e_recente(Data d1, Hora h1, Data d2, Hora h2) {
    // Verifica se as datas e horas são válidas
    if (!dataValida(d2) || !horaValida(h2))
        return FALSE;

    // Verifica se o instante 2 é mais recente ou igual ao 1
    if (dataRecente(d1, d2)) {
        if (dataIgual(d1, d2)) {
            // Se as datas são iguais, verifica as horas
            if (horaRecente(h1, h2)) {
                return TRUE;
            }
        } else {
            // Se a data 2 é mais recente que a data 1, então é válida
            return TRUE;
        }
    }
    
    return FALSE;
}

/*Retorna o dia seguinte a uma determinada data*/
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

/*Retorna o número de minutos que passaram entre dois instantes de tempo*/
int calcula_minutos_entre_datas(Data d1, Hora h1, Data d2, Hora h2) {
    int diasinc = 0;
    
    while(!dataRecente(d2, d1)){
        diasinc++;
        d1 = datamaismais(d1);
    }

    return diasinc*24*60 + h2.h*60 + h2.min - h1.h*60 -h1.min;
}


/*Atualiza o estado da matrícula na hashtable para DENTRO*/
void atualiza_mat_hashtable_estado_dentro(HashTable *hashTable, char *matricula) {
    // Calcula o índice da matrícula na tabela hash
    int index = hash(matricula);

    // Obtém o nó correspondente à matrícula na tabela hash
    nodeHASH *node = hashTable->table[index];

    // Procura a matrícula na lista encadeada
    while (node != NULL) {
        if (strcmp(node->matricula, matricula) == 0) {
            // Atualiza o estado do veículo para FORA
            node->estado = DENTRO;
            return;
        }
        node = node->next;
    }

    // Se a matrícula não for encontrada, não há nada para atualizar
}

/*Função principal associada à entrada de um veículo num parque*/
int insere_entrada_parque(Parque *parque, char *matricula, Data data,
                          Hora hora, HashTable *hashTable) {
    //Verifica se o parque está cheio, data, matrícula e se o carro está dentro
    if (parque->lugares_disponiveis <= 0) {
        return -1;
    }

    if (!matricula_valida(matricula)) {
        return -2;
    }

    nodeHASH *estado_matricula = procura_na_hastable(hashTable, matricula);
    if (estado_matricula != NULL && estado_matricula->estado == DENTRO) {
        return -3;
    }
    
    if (!data_hora_valida_e_recente(data_atual, hora_atual, data, hora)) {
        return -4;
    }
     
    // Alocar memória para o novo registro
    Registo_entradas *novo_registo = malloc(sizeof(Registo_entradas));
    if (novo_registo == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Preencher os dados do novo registro e insere-o na lista de entradas
    strcpy(novo_registo->matricula, matricula);
    novo_registo->data = data;
    novo_registo->hora = hora;
    novo_registo->estado = LIVRE;
    novo_registo->next = NULL;
    
    if (parque->entradas == NULL) {
        parque->entradas = novo_registo;
    } else {
        Registo_entradas *atual = parque->entradas;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novo_registo;
    }

    // Atualizar o estado do veículo para DENTRO na hashtable de matrículas
    insere_mat_hashtable(hashTable, matricula);
    atualiza_mat_hashtable_estado_dentro(hashTable, matricula);
    // Subtrair 1 do número de lugares disponíveis
    parque->lugares_disponiveis--;
    atualizar_tempo(&data_atual, &hora_atual, data, hora);

    return 0;
}

/*Processa o input do comando "e" dado pelo utilizador.*/
void processar_entradas(HashTable *hashTable) {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;

    leLinha(argumentos, &n_argumentos);

    if (n_argumentos == 4) {
        char *nome_parque = argumentos[0];
        char *matricula = argumentos[1];
        Data data_entrada;
        Hora hora_entrada;

        // Converte a data e hora para o formato desejado
        sscanf(argumentos[2], "%d-%d-%d", &data_entrada.d, &data_entrada.m,
               &data_entrada.a);
        sscanf(argumentos[3], "%d:%d", &hora_entrada.h, &hora_entrada.min);

        // Verifica se o parque existe
        int indice_parque = parque_existe(nome_parque);
        if (indice_parque) {
            Parque *parque = &stored_parques[indice_parque - 1];
            
            int resultado = insere_entrada_parque(parque, matricula, data_entrada, 
                                                  hora_entrada, hashTable);
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




/*Calcula o custo do veículo segundo o regime de faturação de um parque*/
float calcular_custo_estadia(float valor_15, float valor_15_apos_1hora,
                             float valor_max_diario, int min_estadia) {
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
    custo_total += dias*valor_max_diario + min(valor_max_diario, min(4, periodos) *
                   valor_15 + max(0, periodos - 4) * valor_15_apos_1hora);
    
    return custo_total;
}


/* Procura pelo registro de entrada mais recente de uma matrícula específica */
Registo_entradas* procura_matricula_parque_recente(Parque *parque, char *matricula) {
    // Verifica se o parque é válido e se há registros de entrada
    if (parque == NULL || parque->entradas == NULL) {
        return NULL;
    }
    Registo_entradas *registro_recente = NULL;

    Registo_entradas *atual = parque->entradas;
    while (atual != NULL) {
        // Compara a matrícula atual com a matrícula procurada
        if (strcmp(atual->matricula, matricula) == 0) {
            // Atualiza o registro mais recente
            registro_recente = atual;
        }
        // Passa para o próximo registro
        atual = atual->next;
    }

    return registro_recente;
}



/*Atualiza o registo na lista de entradas com a info da saída do veículo*/
Registo_entradas* altera_mat_registo_infosaida(Parque *parque, char *matricula,
                                          Data data_saida, Hora hora_saida) {
    // Verifica se o parque é válido
    if (parque == NULL) {
        return NULL;
    }

    // Percorre a lista de registros de entrada do parque
    Registo_entradas *atual = parque->entradas;
    while (atual != NULL) {
        // Compara a matrícula atual com a matrícula procurada
        if (strcmp(atual->matricula, matricula) == 0 && atual->estado == LIVRE) {
            //Atualiza as informações do veículo em questão
            atual->estado = CONECTADO;
            atual->data_saida = data_saida;
            atual->hora_saida = hora_saida;
        }
        // Passa para o próximo registro
        atual = atual->next;
    }

    // Retorna NULL se a matrícula não for encontrada
    return NULL;
}

/*Atualiza o estado da matrícula na hashtable para FORA*/
void atualiza_mat_hashtable_estado_fora(HashTable *hashTable, char *matricula) {
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




void atualizar_custo_registro_saida(Parque *parque, char *matricula, Data data,
                                    Hora hora, float novo_custo) {
    // Percorre a lista de registros de saída do parque
    Registo_saidas *atual = parque->saidas;
    while (atual != NULL) {
        // Compara a matrícula, data e hora do registro atual com os fornecidos
        if (strcmp(atual->matricula, matricula) == 0 &&
            dataIgual(atual->data, data) && horaIgual(atual->hora, hora)) {
            // Atualiza o custo do registro de saída para o novo valor
            atual->custo = novo_custo;
            return;
        }
        // Passa para o próximo registro
        atual = atual->next;
    }
}


/*Função principal associada à saida de um veículo de um parque*/
int insere_saida_parque(Parque *parque, char *matricula, Data data,
                        Hora hora, HashTable *hashTable) {

    // Verifica a matrícula, data e se o carro está fora do parque
    if (!matricula_valida(matricula)) {
        return -1;
    }

    nodeHASH *estado_matricula = procura_na_hastable(hashTable, matricula);
    if (estado_matricula != NULL && estado_matricula->estado == FORA) {
        return -2;
    }
    
    if (!data_hora_valida_e_recente(data_atual, hora_atual, data, hora)) {
        return -3;
    }

    // Alocar memória para o novo registro
    Registo_saidas *novo_registo_saida = malloc(sizeof(Registo_saidas));
    if (novo_registo_saida == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Preencher os dados do novo registro e insere-o na lista de saidas
    strcpy(novo_registo_saida->matricula, matricula);
    novo_registo_saida->data = data;
    novo_registo_saida->hora = hora;
    novo_registo_saida->next = NULL;

    if (parque->saidas == NULL) {
        parque->saidas = novo_registo_saida;
    } else {
        Registo_saidas *atual = parque->saidas;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = novo_registo_saida;
    }

    // Atualiza as informações do registro de entrada com a saída do veículo
    altera_mat_registo_infosaida(parque, matricula, data, hora);
    // Atualizar o estado do veículo para FORA na hashtable de matrículas
    atualiza_mat_hashtable_estado_fora(hashTable, matricula);
    parque->lugares_disponiveis++;
    atualizar_tempo(&data_atual, &hora_atual, data, hora);

    return 0;
}


/*Processa o input do comando "s" dado pelo utilizador.*/
void processar_saidas(HashTable *hashTable) {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;

    leLinha(argumentos, &n_argumentos);

    if (n_argumentos == 4) {
        char *nome_parque = argumentos[0];
        char *matricula = argumentos[1];
        Data data_saida;
        Hora hora_saida;

        // Converte a data e hora de saída para o formato desejado
        sscanf(argumentos[2], "%d-%d-%d", &data_saida.d, &data_saida.m, &data_saida.a);
        sscanf(argumentos[3], "%d:%d", &hora_saida.h, &hora_saida.min);

        // Verifica se o parque existe, se existir processa o input dado
        int indice_parque = parque_existe(nome_parque);
        if (indice_parque) {
            Parque *parque = &stored_parques[indice_parque - 1];
            
            int resultado = insere_saida_parque(parque, matricula, data_saida,
                                                hora_saida, hashTable);
            if (resultado == 0) {
                Registo_entradas *registro = procura_matricula_parque_recente(parque, matricula);

                Data data_entrada = registro->data;
                Hora hora_entrada = registro->hora;
                
                int minutos_estadia = calcula_minutos_entre_datas(data_entrada, hora_entrada,
                                                                  data_saida, hora_saida);
                float custo_estadia = calcular_custo_estadia(parque->valor_15,
                parque->valor_15_apos_1hora, parque->valor_max_diario, minutos_estadia);

                atualizar_custo_registro_saida(parque, matricula, data_saida,
                                               hora_saida, custo_estadia);

                // Imprime a informação sobre o parque após cada saida.
                printf("%s %02d-%02d-%d %02d:%02d %02d-%02d-%d %02d:%02d %.2f\n", matricula,
               data_entrada.d, data_entrada.m, data_entrada.a,
               hora_entrada.h, hora_entrada.min,
               registro->data_saida.d, registro->data_saida.m, registro->data_saida.a,
               registro->hora_saida.h, registro->hora_saida.min,
               custo_estadia);
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



void mostrar_faturacao_diaria(char *nome_parque, Data data_pesquisa) {
    if (!dataRecente(data_pesquisa, data_atual) || !dataValida(data_pesquisa)) {
        printf("invalid date.\n");
        return;
    }
    
    int indice_parque = parque_existe(nome_parque);
    if (indice_parque) {
        Parque *parque = &stored_parques[indice_parque - 1];
        Registo_saidas *atual = parque->saidas;
        while (atual != NULL) {
            if (dataIgual(atual->data, data_pesquisa)) {
                printf("%s %02d:%02d %.2f\n", atual->matricula, atual->hora.h, atual->hora.min, atual->custo);
            }
            atual = atual->next;
        }
    } else {
        printf("%s: no such parking.\n", nome_parque);
    }
}

void mostrar_faturacao_total(char *nome_parque) {
    int indice_parque = parque_existe(nome_parque);
    if (indice_parque) {
        Parque *parque = &stored_parques[indice_parque - 1];
        Registo_saidas *atual = parque->saidas;
        Data data_anterior = {0, 0, 0}; // Inicializa a data anterior para comparar com a primeira data da lista
        
        float faturacao_total = 0.0;
        
        while (atual != NULL) {
            // Verifica se a data do registro atual é diferente da anterior
            if (!dataIgual(atual->data, data_anterior)) {
                // Se a data for diferente e não for a primeira, imprime a faturação total da data anterior
                if (data_anterior.a != 0) {
                    printf("%02d-%02d-%02d %.2f\n", data_anterior.d, data_anterior.m, data_anterior.a, faturacao_total);
                }
                // Reinicia a faturação total para a nova data
                faturacao_total = 0.0;
            }
            // Adiciona o custo do registro atual à faturação total
            faturacao_total += atual->custo;
            // Atualiza a data anterior para a data atual
            data_anterior = atual->data;
            // Move para o próximo registro
            atual = atual->next;
        }
        
        // Imprime a faturação total da última data
        if (data_anterior.a != 0) {
            printf("%02d-%02d-%02d %.2f\n", data_anterior.d, data_anterior.m, data_anterior.a, faturacao_total);
        }
    } else {
        printf("%s: no such parking.\n", nome_parque);
    }
}


void processar_faturacao() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;

    leLinha(argumentos, &n_argumentos);

    /*Com 1 argumento, mostra a faturação diária de um parque*/
    if (n_argumentos == 1) {
        char *nome_parque = argumentos[0];
        mostrar_faturacao_total(nome_parque);
    }
    
    /*Com 2 argumentos, mostra a faturação de um parque num dia concreto*/
    else if (n_argumentos == 2) {
        char *nome_parque = argumentos[0];
        Data data_pesquisa;

        // Converte a data e hora de saída para o formato desejado
        sscanf(argumentos[1], "%d-%d-%d", &data_pesquisa.d, &data_pesquisa.m,
               &data_pesquisa.a);
        
        mostrar_faturacao_diaria(nome_parque, data_pesquisa);
        
    }
}




void merge(int arr[], int l, int m, int r, Parque stored_parques[MAX_PARQUES]){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Cria arrays temporários
    int L[n1], R[n2];
    
    // Copia os dados para os arrays temporários L[] e R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Mescla os arrays temporários de volta em arr[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (strcmp(stored_parques[L[i]].nome_parque, stored_parques[R[j]].nome_parque) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copia os elementos restantes de L[], se houver
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copia os elementos restantes de R[], se houver
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r, Parque stored_parques[MAX_PARQUES]) {
    if (l < r) {
        // Encontra o ponto médio
        int m = l + (r - l) / 2;

        // Ordena a primeira metade e a segunda metade
        mergeSort(arr, l, m, stored_parques);
        mergeSort(arr, m + 1, r, stored_parques);

        // Mescla as partes ordenadas
        merge(arr, l, m, r, stored_parques);
    }
}


void mostrar_registros_veiculo(char *matricula) {
    int encontrou_registros = 0;
    int indices_parques[N_parques]; // Array de índices para os parques
    // Preenche o array de índices com os índices dos parques
    for (int i = 0; i < N_parques; i++) {
        indices_parques[i] = i;
    }
    // Ordena os índices dos parques pelo nome do parque
    mergeSort(indices_parques, 0, N_parques - 1, stored_parques);

    // Percorre todos os parques
    for (int i = 0; i < N_parques; i++) {
        Parque *parque = &stored_parques[indices_parques[i]];
        // Procura por entradas e saídas do veículo no parque atual
        Registo_entradas *registro = parque->entradas;
        while (registro != NULL) {
            // Se encontrou um registro do veículo, imprime as informações
            if (strcmp(registro->matricula, matricula) == 0) {
                encontrou_registros = 1;
                printf("%s %02d-%02d-%d %02d:%02d", parque->nome_parque, registro->data.d, registro->data.m, registro->data.a, registro->hora.h, registro->hora.min);
                // Se o veículo está atualmente dentro do parque, não mostra a data e hora de saída
                if (registro->estado == LIVRE) {
                    printf("\n");
                } else {
                    printf(" %02d-%02d-%d %02d:%02d\n", registro->data_saida.d, registro->data_saida.m, registro->data_saida.a, registro->hora_saida.h, registro->hora_saida.min);
                }
            }
            registro = registro->next;
        }
    }
    // Se nenhum registro foi encontrado, imprime mensagem de erro
    if (!encontrou_registros) {
        printf("%s: no entries found in any parking.\n", matricula);
    }
}

void processar_comando_v() {
    char argumentos[MAX_ARGUMENTOS][MAX_INPUT];
    int n_argumentos;
    // Lê a linha de comando
    leLinha(argumentos, &n_argumentos);

    char *matricula = argumentos[0];

    // Verifica se a matrícula é válida
    if (!matricula_valida(matricula)) {
        printf("%s: invalid licence plate.\n", matricula);
        return;
    }

    // Chama a função para mostrar os registros do veículo
    mostrar_registros_veiculo(matricula);
}







//vamos procurar muitas vezes as entradas e saidas dos carros.
//começar por onde da speed up e usar tabelas de dispersão

//hastables para entradas e saidas


/*FUNÇÕES PARA LIBERTAR MEMÓRIA*/
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

/*
void destroy_node_entradas(Registo_entradas *registo) {
    if (registo != NULL) {
        destroy_node_entradas(registo->next);
        free(registo->matricula);
        registo->matricula = NULL;
        free(registo);
        registo = NULL;
    }
}

void destroy_node_saidas(Registo_saidas *registo) {
    if (registo != NULL) {
        destroy_node_saidas(registo->next);
        free(registo->matricula);
        registo->matricula = NULL;
        free(registo);
        registo = NULL;
    }
}

void liberta(Parque stored_parques[MAX_PARQUES], HashTable *hashTable) {
    for (int i = 0; i < MAX_PARQUES; i++) {
        free(stored_parques[i].nome_parque);
        stored_parques[i].nome_parque = NULL;
        destroy_node_entradas(stored_parques[i].head_ent);
        destroy_node_saidas(stored_parques[i].head_sai);
    }

    liberta_hashtable(hashTable);
}
*/




/* CENAS PARA FAZER

//COMANDO e
-

//COMANDO s
-

//COMANDO v
- Ordenar primeiro a lista de parques pelo nome
- Iterar pelo registo de entradas de cada parque e filtrar pela matrícula
--> Se estado == CONECTADO, então imprime data/hora entrada e data/hora saida
--> Se estado == LIVRE, então imprime só data/hora entrada

//COMANDO f
- 

*/