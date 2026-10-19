#include "paleta.h"
#include <stdio.h>
#include <stdlib.h>

void paleta_iniciar(Paleta *paleta) {
    paleta->inicio = NULL;
    paleta->quantidade = 0;
}

bool ler_cor(const char *texto, Cor *cor) {
    int r, g, b;
    char sobra;

    if (sscanf(texto, "%d,%d,%d%c", &r, &g, &b, &sobra) != 3) {
        return false;
    }
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return false;
    }
    if ((r == 0 && g == 0 && b == 0) ||
        (r == 255 && g == 255 && b == 255)) {
        return false;
    }

    cor->r = (unsigned char)r;
    cor->g = (unsigned char)g;
    cor->b = (unsigned char)b;
    return true;
}

bool paleta_adicionar(Paleta *paleta, Cor cor) {
    NoCor *novo = malloc(sizeof(*novo));
    if (novo == NULL) {
        return false;
    }
    novo->cor = cor;

    if (paleta->inicio == NULL) {
        novo->proxima = novo;
        paleta->inicio = novo;
    } else {
        NoCor *ultimo = paleta->inicio;
        while (ultimo->proxima != paleta->inicio) {
            ultimo = ultimo->proxima;
        }
        novo->proxima = paleta->inicio;
        ultimo->proxima = novo;
    }
    paleta->quantidade++;
    return true;
}

Cor paleta_obter(const Paleta *paleta, size_t indice) {
    NoCor *atual = paleta->inicio;
    indice %= paleta->quantidade;
    for (size_t i = 0; i < indice; i++) {
        atual = atual->proxima;
    }
    return atual->cor;
}

void paleta_liberar(Paleta *paleta) {
    if (paleta->inicio == NULL) {
        return;
    }
    NoCor *atual = paleta->inicio->proxima;
    while (atual != paleta->inicio) {
        NoCor *proximo = atual->proxima;
        free(atual);
        atual = proximo;
    }
    free(paleta->inicio);
    paleta->inicio = NULL;
    paleta->quantidade = 0;
}
