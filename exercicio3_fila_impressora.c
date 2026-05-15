#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
//  Definição das estruturas
// ============================================================

typedef struct Documento {
    int  codigoProcesso;
    char nomeArquivo[100];
    long quantidadeBytes;
} Documento;

typedef struct No {
    Documento dados;
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

void enfileirar(Fila *f, Documento d) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }
    novo->dados   = d;
    novo->proximo = NULL;

    if (f->fim == NULL) {
        f->inicio = novo;
    } else {
        f->fim->proximo = novo;
    }
    f->fim = novo;
    f->tamanho++;
}

int desenfileirar(Fila *f, Documento *d) {
    if (f->inicio == NULL) return 0;

    No *temp  = f->inicio;
    *d        = temp->dados;
    f->inicio = temp->proximo;
    if (f->inicio == NULL) f->fim = NULL;
    free(temp);
    f->tamanho--;
    return 1;
}

// ============================================================
//  Função auxiliar para criar um Documento
// ============================================================

Documento criarDocumento(int codigo, const char *nome, long bytes) {
    Documento d;
    d.codigoProcesso  = codigo;
    strncpy(d.nomeArquivo, nome, sizeof(d.nomeArquivo) - 1);
    d.nomeArquivo[sizeof(d.nomeArquivo) - 1] = '\0';
    d.quantidadeBytes = bytes;
    return d;
}

// ============================================================
//  Função auxiliar para exibir tamanho em unidade legível
// ============================================================

void exibirTamanho(long bytes) {
    if (bytes >= 1048576) {
        printf("%.2f MB (%ld bytes)", (double)bytes / 1048576, bytes);
    } else if (bytes >= 1024) {
        printf("%.2f KB (%ld bytes)", (double)bytes / 1024, bytes);
    } else {
        printf("%ld bytes", bytes);
    }
}

// ============================================================
//  Main
// ============================================================

int main(void) {
    Fila fila;
    inicializarFila(&fila);

    // --- Enfileira 5 documentos ---
    enfileirar(&fila, criarDocumento(1001, "relatorio_anual.pdf",    2097152));  // ~2 MB
    enfileirar(&fila, criarDocumento(1002, "contrato_servico.docx",   512000));  // ~500 KB
    enfileirar(&fila, criarDocumento(1003, "planilha_orcamento.xlsx", 348160));  // ~340 KB
    enfileirar(&fila, criarDocumento(1004, "apresentacao_vendas.pptx",1572864)); // ~1.5 MB
    enfileirar(&fila, criarDocumento(1005, "memo_interno.txt",          2048));  // ~2 KB

    printf("=================================================\n");
    printf("       Simulacao de Fila de Impressora\n");
    printf("=================================================\n");
    printf("Documentos na fila: %d\n\n", fila.tamanho);

    int ordem = 1;
    Documento imprimindo;

    // --- Retirada e impressão das informações ---
    while (desenfileirar(&fila, &imprimindo)) {
        printf("-------------------------------------------------\n");
        printf("Imprimindo documento %d de 5\n", ordem++);
        printf("  Codigo do processo : %d\n",    imprimindo.codigoProcesso);
        printf("  Nome do arquivo    : %s\n",    imprimindo.nomeArquivo);
        printf("  Tamanho            : ");
        exibirTamanho(imprimindo.quantidadeBytes);
        printf("\n");
        printf("  Status             : Concluido\n");
    }

    printf("-------------------------------------------------\n");
    printf("\nFila vazia. Todos os documentos foram impressos.\n");

    return 0;
}
