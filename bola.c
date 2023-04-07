#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "bola.h"

ball *bolaCria()
{
    ball *bola;
    if ((bola = malloc(sizeof(ball))))
    {
        bola->ini = NULL;
        bola->fim = NULL;
        bola->qntBolas = 0;
        return bola;
    }
    return NULL;
}

int zeroBolas(ball *bola)
{
    if ((bola->ini == NULL) && (bola->qntBolas == 0))
        return 1;
    return 0;
}

void bolaInsere(ball *bola, inicio rodadaAtual)
{
    b_uni *b;
    b_uni *ptr = bola->ini;
    if ((b = malloc(sizeof(b_uni))))
    {
        b->prox = NULL;
        b->ingame = 0;
        b->rodou = 1;
        b->x = rodadaAtual.x;
        b->y = rodadaAtual.y;

        if (zeroBolas(bola))
        {
            b->elemento = 0;
            bola->ini = b;
            bola->fim = b;
        }
        else
        {
            while (ptr->prox != NULL)
                ptr = ptr->prox;
            b->elemento = bola->qntBolas;
            ptr->prox = b;
            bola->fim = b;
        }
    }
    (bola->qntBolas)++;
}

void bolaDestroi(ball *bola)
{
    b_uni *ptr = bola->ini;
    if (zeroBolas(bola))
    {
        free(bola);
    }
    else
    {
        while (bola->ini->prox != NULL)
        {
            ptr = bola->ini;
            bola->ini = ptr->prox;
            free(ptr);
        }
        bola->qntBolas = 0;
        free(bola->ini);
        free(bola);
    }
}
