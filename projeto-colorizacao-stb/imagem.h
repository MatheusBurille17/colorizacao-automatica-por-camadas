#ifndef IMAGEM_H
#define IMAGEM_H

#include <stdbool.h>
#include <stddef.h>
#include "paleta.h"

#define MAX_DIMENSAO 128

typedef struct {
    int largura;
    int altura;
    unsigned char *pixels; /* RGBA: quatro bytes por pixel. */
} Imagem;

bool imagem_carregar(const char *caminho, Imagem *imagem);
bool imagem_validar(const Imagem *imagem);
bool imagem_colorir(Imagem *imagem, const Paleta *paleta, size_t *regioes);
bool imagem_salvar_png(const char *caminho, const Imagem *imagem);
void imagem_liberar(Imagem *imagem);

#endif
