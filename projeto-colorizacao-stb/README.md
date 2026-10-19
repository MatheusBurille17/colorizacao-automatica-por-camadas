# Projeto de colorizacao automatica por camadas

Implementacao em C usando a fila encadeada e a paleta circular. A leitura e gravacao de imagens usam `stb_image` e `stb_image_write`, conforme o exemplo de suporte do exercicio.

## Estrutura

- `main.c`: argumentos, validacao da paleta e fluxo principal.
- `fila.c` / `fila.h`: fila encadeada usada na busca em largura (BFS).
- `paleta.c` / `paleta.h`: leitura RGB e lista circular de cores.
- `imagem.c` / `imagem.h`: carregamento, validacao, colorizacao e salvamento.
- `image_codecs.c`: implementacao da biblioteca stb, compilada uma unica vez.
- `vendor/`: cabecalhos originais da biblioteca e licenca.
- `entradas/`: entrada das imagens png preto e branco.
- `saidas/`: resultado das imagens png coloridas.

## Requisitos

Imagem de entrada com largura e altura de no maximo 128 pixels, opaca e contendo somente pixels pretos ou brancos. As cores da paleta devem estar no formato `R,G,B`, com valores de 0 a 255, e nao podem ser preto puro nem branco puro. Sao necessarias pelo menos duas cores; cores repetidas sao permitidas.

## Compilar

Execute no terminal, dentro desta pasta:

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -isystem vendor main.c fila.c paleta.c imagem.c image_codecs.c -o colorizacao -lm
```

No Windows, o executavel gerado sera `colorizacao.exe`.

## Executar

```powershell
.\colorizacao.exe entrada.png saida.png 255,0,0 0,0,255 255,255,0
```

## Exemplos

Estrela arco iris:

```powershell
.\colorizacao.exe entradas\estrela.png saidas\estrela_arco_iris.png 255,0,0 255,127,0 255,255,0 0,200,0 0,100,255 75,0,130 148,0,211
```

Coração Vermelho:

```powershell
.\colorizacao.exe entradas\coracao.png saidas\coracao_vermelho.png 255,0,0 220,20,60
```

O programa percorre os pixels em ordem de linha e coluna. Cada regiao branca conexa por cima/baixo/esquerda/direita comeca na primeira posicao ainda nao visitada. A distancia dentro da regiao e calculada por BFS, e a cor e escolhida por `distancia % quantidade_de_cores`. Pixels pretos sao preservados. A saida e sempre PNG.
