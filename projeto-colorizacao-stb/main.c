#include "imagem.h"
#include "paleta.h"

#include <stdio.h>
#include <stdlib.h>

static void mostrar_uso(const char *programa) {
    fprintf(stderr, "Uso: %s entrada.png saida.png R,G,B R,G,B [R,G,B ...]\n", programa);
    fprintf(stderr, "Informe pelo menos duas cores; preto e branco nao sao permitidos.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        mostrar_uso(argv[0]);
        return EXIT_FAILURE;
    }

    Paleta paleta;
    paleta_iniciar(&paleta);
    for (int i = 3; i < argc; i++) {
        Cor cor;
        if (!ler_cor(argv[i], &cor)) {
            fprintf(stderr, "Cor invalida: %s (use R,G,B com valores de 0 a 255, sem preto/branco).\n", argv[i]);
            paleta_liberar(&paleta);
            return EXIT_FAILURE;
        }
        if (!paleta_adicionar(&paleta, cor)) {
            fprintf(stderr, "Sem memoria para montar a paleta.\n");
            paleta_liberar(&paleta);
            return EXIT_FAILURE;
        }
    }
    if (paleta.quantidade < 2) {
        fprintf(stderr, "A paleta precisa ter pelo menos duas cores.\n");
        paleta_liberar(&paleta);
        return EXIT_FAILURE;
    }

    Imagem imagem = {0};
    if (!imagem_carregar(argv[1], &imagem)) {
        paleta_liberar(&paleta);
        return EXIT_FAILURE;
    }
    if (!imagem_validar(&imagem)) {
        imagem_liberar(&imagem);
        paleta_liberar(&paleta);
        return EXIT_FAILURE;
    }

    size_t regioes = 0;
    if (!imagem_colorir(&imagem, &paleta, &regioes)) {
        imagem_liberar(&imagem);
        paleta_liberar(&paleta);
        return EXIT_FAILURE;
    }
    if (!imagem_salvar_png(argv[2], &imagem)) {
        imagem_liberar(&imagem);
        paleta_liberar(&paleta);
        return EXIT_FAILURE;
    }

    printf("Regioes coloridas: %zu\n", regioes);
    printf("Ordem da paleta: ");
    for (size_t i = 0; i < paleta.quantidade; i++) {
        Cor cor = paleta_obter(&paleta, i);
        printf("(%u,%u,%u)%s", (unsigned)cor.r, (unsigned)cor.g, (unsigned)cor.b,
               i + 1 < paleta.quantidade ? " -> " : "\n");
    }
    printf("Imagem salva em: %s\n", argv[2]);

    imagem_liberar(&imagem);
    paleta_liberar(&paleta);
    return EXIT_SUCCESS;
}
