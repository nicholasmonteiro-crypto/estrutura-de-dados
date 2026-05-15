#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
//  Definição das estruturas
// ============================================================

typedef struct Cliente {
    char nome[100];
    int  tempoAtendimento; // em minutos (gerado aleatoriamente)
} Cliente;

typedef struct No {
    Cliente dados;
    struct No *proximo;
} No;

typedef struct {
    No *inicio;
    No *fim;
    int tamanho;
} Fila;

// ============================================================
//  Funções da Fila
// ============================================================

void inicializarFila(Fila *f) {
    f->inicio  = NULL;
    f->fim     = NULL;
    f->tamanho = 0;
}

void enfileirar(Fila *f, Cliente c) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memória insuficiente.\n");
        exit(1);
    }
    novo->dados   = c;
    novo->proximo = NULL;

    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->proximo = novo;
    }
    f->fim = novo;
    f->tamanho++;
}

int desenfileirar(Fila *f, Cliente *c) {
    if (f->inicio == NULL) return 0;

    No *temp  = f->inicio;
    *c        = temp->dados;
    f->inicio = temp->proximo;
    if (f->inicio == NULL) f->fim = NULL;
    free(temp);
    f->tamanho--;
    return 1;
}

// ============================================================
//  Função auxiliar para criar um Cliente
// ============================================================

// Tempo aleatório entre min e max minutos
int tempoAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

Cliente criarCliente(const char *nome, int tempoMin, int tempoMax) {
    Cliente c;
    strncpy(c.nome, nome, sizeof(c.nome) - 1);
    c.nome[sizeof(c.nome) - 1] = '\0';
    c.tempoAtendimento = tempoAleatorio(tempoMin, tempoMax);
    return c;
}

// ============================================================
//  Main
// ============================================================

int main(void) {
    srand((unsigned int)time(NULL)); // semente para números aleatórios

    Fila fila;
    inicializarFila(&fila);

    // --- Enfileira 7 clientes (tempo aleatório entre 3 e 15 min) ---
    enfileirar(&fila, criarCliente("Ana Lima",        3, 15));
    enfileirar(&fila, criarCliente("Bruno Souza",     3, 15));
    enfileirar(&fila, criarCliente("Carla Mendes",    3, 15));
    enfileirar(&fila, criarCliente("Diego Rocha",     3, 15));
    enfileirar(&fila, criarCliente("Elisa Ferreira",  3, 15));
    enfileirar(&fila, criarCliente("Fabio Castro",    3, 15));
    enfileirar(&fila, criarCliente("Giovana Alves",   3, 15));

    printf("=========================================\n");
    printf("   Simulacao de Fila Bancaria\n");
    printf("=========================================\n\n");
    printf("%-22s | Tempo de Atendimento\n", "Cliente");
    printf("----------------------+---------------------\n");

    int totalTempo  = 0;
    int totalPessoas = 0;
    Cliente atendido;

    // --- Retirada e acúmulo de tempo ---
    while (desenfileirar(&fila, &atendido)) {
        printf("%-22s | %d minuto(s)\n",
               atendido.nome,
               atendido.tempoAtendimento);
        totalTempo += atendido.tempoAtendimento;
        totalPessoas++;
    }

    // --- Cálculo da média ---
    double media = (double)totalTempo / totalPessoas;

    printf("\n=========================================\n");
    printf("Total de clientes atendidos : %d\n", totalPessoas);
    printf("Tempo total de atendimento  : %d minuto(s)\n", totalTempo);
    printf("Media de atendimento        : %.2f minuto(s)\n", media);
    printf("=========================================\n\n");

    // --- Recomendação ---
    if (media > 8.0) {
        printf("RECOMENDACAO: A media de atendimento (%.2f min) ultrapassou\n"
               "o limite de 8 minutos. Recomenda-se a abertura de um novo\n"
               "caixa para melhorar o atendimento no horario de pico.\n", media);
    } else {
        printf("RECOMENDACAO: A media de atendimento (%.2f min) esta dentro\n"
               "do limite de 8 minutos. Nao e necessario abrir um novo caixa\n"
               "no horario de pico.\n", media);
    }

    return 0;
}
