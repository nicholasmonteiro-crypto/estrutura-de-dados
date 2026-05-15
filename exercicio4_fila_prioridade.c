#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
//  Definição das estruturas
// ============================================================

typedef struct Pessoa {
    char nome[100];
    int  idade;
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
//  Funções básicas da Fila
// ============================================================

void inicializarFila(Fila *f) {
    f->inicio  = NULL;
    f->fim     = NULL;
    f->tamanho = 0;
}

// Insere no FIM (ordem normal)
void enfileirar(Fila *f, Pessoa p) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) { printf("Erro: memoria insuficiente.\n"); exit(1); }
    novo->dados   = p;
    novo->proximo = NULL;

    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->proximo = novo;
    }
    f->fim = novo;
    f->tamanho++;
}

// Insere no INÍCIO (para prioridade)
void enfileirarPrioritario(Fila *f, Pessoa p) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) { printf("Erro: memoria insuficiente.\n"); exit(1); }
    novo->dados   = p;
    novo->proximo = f->inicio;

    f->inicio = novo;
    if (f->fim == NULL) f->fim = novo; // fila estava vazia
    f->tamanho++;
}

int desenfileirar(Fila *f, Pessoa *p) {
    if (f->inicio == NULL) return 0;

    No *temp  = f->inicio;
    *p        = temp->dados;
    f->inicio = temp->proximo;
    if (f->inicio == NULL) f->fim = NULL;
    free(temp);
    f->tamanho--;
    return 1;
}

// ============================================================
//  Enfileirar com verificação de prioridade
//  - Se idade > 65: insere à frente do primeiro NÃO prioritário
//    (mantendo a ordem relativa entre prioritários já na fila)
//  - Caso contrário: insere no fim normalmente
// ============================================================

void enfileirarComPrioridade(Fila *f, Pessoa p) {
    if (p.idade > 65) {
        // Encontra a posição logo após o último prioritário já enfileirado
        No *anterior = NULL;
        No *atual    = f->inicio;

        while (atual != NULL && atual->dados.idade > 65) {
            anterior = atual;
            atual    = atual->proximo;
        }

        No *novo = (No *)malloc(sizeof(No));
        if (novo == NULL) { printf("Erro: memoria insuficiente.\n"); exit(1); }
        novo->dados   = p;
        novo->proximo = atual;

        if (anterior == NULL) {
            // Todos na fila eram não-prioritários (ou fila vazia): insere no início
            f->inicio = novo;
        } else {
            anterior->proximo = novo;
        }
        if (atual == NULL) f->fim = novo; // inseriu no final
        f->tamanho++;
    } else {
        enfileirar(f, p); // não prioritário: vai para o fim
    }
}

// ============================================================
//  Função auxiliar
// ============================================================

Pessoa criarPessoa(const char *nome, int idade) {
    Pessoa p;
    strncpy(p.nome, nome, sizeof(p.nome) - 1);
    p.nome[sizeof(p.nome) - 1] = '\0';
    p.idade = idade;
    return p;
}

void imprimirFila(Fila *f) {
    No *atual = f->inicio;
    int pos   = 1;
    while (atual != NULL) {
        printf("  %d. %-20s | %d anos%s\n",
               pos++,
               atual->dados.nome,
               atual->dados.idade,
               atual->dados.idade > 65 ? "  [PRIORITARIO]" : "");
        atual = atual->proximo;
    }
}

// ============================================================
//  Main
// ============================================================

int main(void) {
    Fila fila;
    inicializarFila(&fila);

    // --- Cadastro das 5 pessoas (ordem de chegada) ---
    // Duas delas são prioritárias (> 65): Dona Maria (70) e Seu Carlos (80)
    Pessoa chegadas[5] = {
        {"Lucas Andrade",   30},  // normal
        {"Patricia Neves",  45},  // normal
        {"Maria das Dores", 70},  // PRIORITARIA
        {"Roberto Lima",    52},  // normal
        {"Carlos Pereira",  80}   // PRIORITARIA
    };

    printf("=================================================\n");
    printf("   Simulacao de Fila com Atendimento Prioritario\n");
    printf("=================================================\n\n");

    printf(">>> Chegada das pessoas (ordem de entrada):\n\n");

    for (int i = 0; i < 5; i++) {
        printf("  Chegou: %-20s | %d anos%s\n",
               chegadas[i].nome,
               chegadas[i].idade,
               chegadas[i].idade > 65 ? "  [PRIORITARIO]" : "");
        enfileirarComPrioridade(&fila, chegadas[i]);
    }

    printf("\n>>> Fila organizada (com prioridade aplicada):\n\n");
    imprimirFila(&fila);

    printf("\n>>> Inicio do atendimento:\n\n");

    int ordem = 1;
    Pessoa atendido;

    while (desenfileirar(&fila, &atendido)) {
        printf("  [%d°] Atendendo: %-20s | %d anos%s\n",
               ordem++,
               atendido.nome,
               atendido.idade,
               atendido.idade > 65 ? "  [PRIORITARIO]" : "");
    }

    printf("\nFila vazia. Atendimento encerrado.\n");
    printf("=================================================\n");

    return 0;
}
