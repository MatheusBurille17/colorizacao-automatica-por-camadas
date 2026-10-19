#ifndef PALETA_H
#define PALETA_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Cor;

typedef struct NoCor {
    Cor cor;
    struct NoCor *proxima;
} NoCor;

typedef struct {
    NoCor *inicio;
    size_t quantidade;
} Paleta;

void paleta_iniciar(Paleta *paleta);
bool ler_cor(const char *texto, Cor *cor);
bool paleta_adicionar(Paleta *paleta, Cor cor);
Cor paleta_obter(const Paleta *paleta, size_t indice);
void paleta_liberar(Paleta *paleta);

#endif
