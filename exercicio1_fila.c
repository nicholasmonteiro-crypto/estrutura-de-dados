#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
//  Definição das estruturas
// ============================================================

typedef struct Pessoa {
    char nome[100];
    char cpf[15];          // formato: 000.000.000-00
    char dataNasc[11];     // formato: DD/MM/AAAA
    char motivoVisita[10]; // "Reclamar" ou "Elogiar"
} Pessoa;

typedef struct No {
    Pessoa dados;
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
    f->inicio = NULL;
    f->fim    = NULL;
    f->tamanho = 0;
}

// Enfileira (insere no fim)
void enfileirar(Fila *f, Pessoa p) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memória insuficiente.\n");
        exit(1);
    }
    novo->dados   = p;
    novo->proximo = NULL;

    if (f->fim == NULL) {          // fila estava vazia
        f->inicio = novo;
    } else {
        f->fim->proximo = novo;
    }
    f->fim = novo;
    f->tamanho++;
}

// Desenfileira (retira do início) — retorna 1 se OK, 0 se vazia
int desenfileirar(Fila *f, Pessoa *p) {
    if (f->inicio == NULL) {
        return 0;
    }
    No *temp   = f->inicio;
    *p         = temp->dados;
    f->inicio  = temp->proximo;
    if (f->inicio == NULL) {       // fila ficou vazia
        f->fim = NULL;
    }
    free(temp);
    f->tamanho--;
    return 1;
}

int filaVazia(Fila *f) {
    return f->inicio == NULL;
}

// ============================================================
//  Função auxiliar para criar uma Pessoa
// ============================================================

Pessoa criarPessoa(const char *nome, const char *cpf,
                   const char *dataNasc, const char *motivo) {
    Pessoa p;
    strncpy(p.nome,         nome,     sizeof(p.nome)     - 1);
    strncpy(p.cpf,          cpf,      sizeof(p.cpf)      - 1);
    strncpy(p.dataNasc,     dataNasc, sizeof(p.dataNasc) - 1);
    strncpy(p.motivoVisita, motivo,   sizeof(p.motivoVisita) - 1);
    // garantir terminação
    p.nome[sizeof(p.nome) - 1]               = '\0';
    p.cpf[sizeof(p.cpf) - 1]                 = '\0';
    p.dataNasc[sizeof(p.dataNasc) - 1]       = '\0';
    p.motivoVisita[sizeof(p.motivoVisita) - 1] = '\0';
    return p;
}

// ============================================================
//  Main
// ============================================================

int main(void) {
    Fila fila;
    inicializarFila(&fila);

    // --- Inserção de 6 pessoas na fila ---
    enfileirar(&fila, criarPessoa("Ana Lima",      "111.111.111-11", "10/03/1990", "Reclamar"));
    enfileirar(&fila, criarPessoa("Bruno Souza",   "222.222.222-22", "25/07/1985", "Elogiar" ));
    enfileirar(&fila, criarPessoa("Carla Mendes",  "333.333.333-33", "14/11/2000", "Reclamar"));
    enfileirar(&fila, criarPessoa("Diego Rocha",   "444.444.444-44", "02/05/1995", "Elogiar" ));
    enfileirar(&fila, criarPessoa("Elisa Ferreira","555.555.555-55", "30/01/1988", "Reclamar"));
    enfileirar(&fila, criarPessoa("Fabio Castro",  "666.666.666-66", "18/09/1993", "Reclamar"));

    printf("=== Atendimento da fila ===\n\n");

    int qtdReclamar = 0;
    int qtdElogiar  = 0;
    Pessoa atendido;

    // --- Retirada e contagem ---
    while (desenfileirar(&fila, &atendido)) {
        printf("Atendendo: %-20s | CPF: %s | Nasc.: %s | Motivo: %s\n",
               atendido.nome,
               atendido.cpf,
               atendido.dataNasc,
               atendido.motivoVisita);

        if (strcmp(atendido.motivoVisita, "Reclamar") == 0) {
            qtdReclamar++;
        } else if (strcmp(atendido.motivoVisita, "Elogiar") == 0) {
            qtdElogiar++;
        }
    }

    // --- Exibição em ordem crescente ---
    printf("\n=== Resultado do dia ===\n");
    if (qtdElogiar <= qtdReclamar) {
        printf("Na fila de hoje tivemos %d pessoa(s) que vieram elogiar e "
               "%d para reclamar.\n", qtdElogiar, qtdReclamar);
    } else {
        printf("Na fila de hoje tivemos %d pessoa(s) que vieram reclamar e "
               "%d para elogiar.\n", qtdReclamar, qtdElogiar);
    }

    return 0;
}
