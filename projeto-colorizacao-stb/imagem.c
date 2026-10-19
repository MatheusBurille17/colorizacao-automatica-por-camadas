#include "imagem.h"
#include "fila.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stb_image.h>
#include <stb_image_write.h>

static unsigned char *pixel_em(Imagem *imagem, int linha, int coluna) {
    size_t indice = ((size_t)linha * (size_t)imagem->largura + (size_t)coluna) * 4U;
    return imagem->pixels + indice;
}

static const unsigned char *pixel_const_em(const Imagem *imagem, int linha, int coluna) {
    size_t indice = ((size_t)linha * (size_t)imagem->largura + (size_t)coluna) * 4U;
    return imagem->pixels + indice;
}

bool imagem_carregar(const char *caminho, Imagem *imagem) {
    int canais_originais = 0;
    imagem->largura = 0;
    imagem->altura = 0;
    imagem->pixels = stbi_load(caminho, &imagem->largura, &imagem->altura,
                               &canais_originais, STBI_rgb_alpha);
    if (imagem->pixels == NULL) {
        const char *motivo = stbi_failure_reason();
        fprintf(stderr, "Erro ao abrir imagem: %s\n",
                motivo != NULL ? motivo : "motivo desconhecido");
        imagem->largura = 0;
        imagem->altura = 0;
        return false;
    }
    return true;
}

bool imagem_validar(const Imagem *imagem) {
    if (imagem->pixels == NULL || imagem->largura <= 0 || imagem->altura <= 0 ||
        imagem->largura > MAX_DIMENSAO || imagem->altura > MAX_DIMENSAO) {
        fprintf(stderr, "A imagem deve ter dimensoes entre 1x1 e 128x128.\n");
        return false;
    }

    size_t total = (size_t)imagem->largura * (size_t)imagem->altura;
    for (size_t i = 0; i < total; i++) {
        const unsigned char *p = imagem->pixels + i * 4U;
        bool preto = p[0] == 0 && p[1] == 0 && p[2] == 0;
        bool branco = p[0] == 255 && p[1] == 255 && p[2] == 255;
        if (p[3] != 255 || (!preto && !branco)) {
            fprintf(stderr, "Imagem invalida: use pixels opacos somente pretos ou brancos.\n");
            return false;
        }
    }
    return true;
}

static bool pixel_branco(const unsigned char *p) {
    return p[0] == 255 && p[1] == 255 && p[2] == 255;
}

static bool adicionar_vizinho(Imagem *imagem, const Paleta *paleta,
                              unsigned char *visitado, Fila *fila,
                              int linha, int coluna, int distancia) {
    size_t indice = (size_t)linha * (size_t)imagem->largura + (size_t)coluna;
    if (visitado[indice]) {
        return true;
    }

    unsigned char *p = pixel_em(imagem, linha, coluna);
    if (!pixel_branco(p)) {
        return true;
    }

    /* Marca ao inserir na fila para nao inserir o mesmo pixel duas vezes. */
    visitado[indice] = 1;
    Cor cor = paleta_obter(paleta, (size_t)distancia);
    p[0] = cor.r;
    p[1] = cor.g;
    p[2] = cor.b;
    if (!fila_enfileirar(fila, linha, coluna, distancia)) {
        fprintf(stderr, "Sem memoria para expandir a regiao.\n");
        return false;
    }
    return true;
}

bool imagem_colorir(Imagem *imagem, const Paleta *paleta, size_t *regioes) {
    size_t total = (size_t)imagem->largura * (size_t)imagem->altura;
    unsigned char *visitado = calloc(total, sizeof(*visitado));
    if (visitado == NULL) {
        fprintf(stderr, "Sem memoria para marcar os pixels visitados.\n");
        return false;
    }

    *regioes = 0;
    Fila fila;
    fila_iniciar(&fila);

    for (int linha = 0; linha < imagem->altura; linha++) {
        for (int coluna = 0; coluna < imagem->largura; coluna++) {
            size_t indice = (size_t)linha * (size_t)imagem->largura + (size_t)coluna;
            if (visitado[indice] || !pixel_branco(pixel_const_em(imagem, linha, coluna))) {
                continue;
            }

            (*regioes)++;
            visitado[indice] = 1;
            Cor primeira = paleta_obter(paleta, 0);
            unsigned char *p = pixel_em(imagem, linha, coluna);
            p[0] = primeira.r;
            p[1] = primeira.g;
            p[2] = primeira.b;
            if (!fila_enfileirar(&fila, linha, coluna, 0)) {
                fprintf(stderr, "Sem memoria para iniciar a regiao.\n");
                fila_liberar(&fila);
                free(visitado);
                return false;
            }

            int atual_linha, atual_coluna, distancia;
            while (fila_desenfileirar(&fila, &atual_linha, &atual_coluna, &distancia)) {
                int proxima_distancia = distancia + 1;
                if (atual_linha > 0 && !adicionar_vizinho(imagem, paleta, visitado, &fila,
                        atual_linha - 1, atual_coluna, proxima_distancia)) goto erro;
                if (atual_linha + 1 < imagem->altura && !adicionar_vizinho(imagem, paleta, visitado, &fila,
                        atual_linha + 1, atual_coluna, proxima_distancia)) goto erro;
                if (atual_coluna > 0 && !adicionar_vizinho(imagem, paleta, visitado, &fila,
                        atual_linha, atual_coluna - 1, proxima_distancia)) goto erro;
                if (atual_coluna + 1 < imagem->largura && !adicionar_vizinho(imagem, paleta, visitado, &fila,
                        atual_linha, atual_coluna + 1, proxima_distancia)) goto erro;
            }
        }
    }

    free(visitado);
    return true;

erro:
    fila_liberar(&fila);
    free(visitado);
    return false;
}

bool imagem_salvar_png(const char *caminho, const Imagem *imagem) {
    int sucesso = stbi_write_png(caminho, imagem->largura, imagem->altura,
                                 STBI_rgb_alpha, imagem->pixels, imagem->largura * 4);
    if (sucesso == 0) {
        fprintf(stderr, "Erro ao salvar PNG: %s\n", caminho);
        return false;
    }
    return true;
}

void imagem_liberar(Imagem *imagem) {
    stbi_image_free(imagem->pixels);
    imagem->pixels = NULL;
    imagem->largura = 0;
    imagem->altura = 0;
}
