#include "fila.h"
#include <stdlib.h>

void fila_iniciar(Fila *fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

bool fila_enfileirar(Fila *fila, int linha, int coluna, int distancia) {
    NoFila *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return false;
    }

    novo->linha = linha;
    novo->coluna = coluna;
    novo->distancia = distancia;
    novo->proximo = NULL;

    if (fila->fim == NULL) {
        fila->inicio = novo;
    } else {
        fila->fim->proximo = novo;
    }
    fila->fim = novo;
    return true;
}

bool fila_desenfileirar(Fila *fila, int *linha, int *coluna, int *distancia) {
    if (fila->inicio == NULL) {
        return false;
    }

    NoFila *removido = fila->inicio;
    *linha = removido->linha;
    *coluna = removido->coluna;
    *distancia = removido->distancia;

    fila->inicio = removido->proximo;
    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }
    free(removido);
    return true;
}

void fila_liberar(Fila *fila) {
    int linha, coluna, distancia;
    while (fila_desenfileirar(fila, &linha, &coluna, &distancia)) {
        /* A remoção libera cada nó da fila. */
    }
}
