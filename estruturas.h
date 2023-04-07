#ifndef _Estruturas_
#define _Estruturas_

#define KEY_SEEN 1
#define KEY_RELEASED 2

#define WIDTH 630
#define HEIGHT 980
#define BORDA_BAIXO 120

#define QDSLARG 7
#define BORDA_QUADRADO 3
#define TAMANHO 1000
#define QDSALT 8
#define TAMANHO_QUADRADO 90
#define MEIO_QUADRADO (TAMANHO_QUADRADO / 2)
#define MAX_PALAVRA 1024

#define CIRCLE_RADIUS (TAMANHO_QUADRADO / 6)
#define BORDA_MINIMA CIRCLE_RADIUS

#define NIVEL_MAX 10000     

typedef struct inicio
{
    double x, y;
    int atualiza;
    int nivel;
} inicio;

struct b
{
    struct b *prox;
    double x;
    double y;
    double dx;
    double dy;
    int ingame;
    int rodou;
    int elemento;
};
typedef struct b b_uni;

struct bola
{
    b_uni *ini;
    b_uni *fim;
    int qntBolas;
};
typedef struct bola ball;

#endif