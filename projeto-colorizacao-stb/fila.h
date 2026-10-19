#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

typedef struct NoFila {
    int linha;
    int coluna;
    int distancia;
    struct NoFila *proximo;
} NoFila;

typedef struct {
    NoFila *inicio;
    NoFila *fim;
} Fila;

void fila_iniciar(Fila *fila);
bool fila_enfileirar(Fila *fila, int linha, int coluna, int distancia);
bool fila_desenfileirar(Fila *fila, int *linha, int *coluna, int *distancia);
void fila_liberar(Fila *fila);

#endif
