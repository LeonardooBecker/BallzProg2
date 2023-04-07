#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "estruturas.h"
#include "menu.h"
#include "ingame.h"
#include "bola.h"
#include "colisao.h"

int numAleatFloat(int inicio, int fim)
{
    return (rand() % (fim - inicio + 1) + inicio);
}

int paredeEsquerda(b_uni *apontaBola)
{
    if (apontaBola->x < BORDA_MINIMA)
    {
        apontaBola->x = CIRCLE_RADIUS;
        apontaBola->dx *= -1;
        return 1;
    }
    return 0;
}

int paredeDireita(b_uni *apontaBola)
{
    if (apontaBola->x > (WIDTH - CIRCLE_RADIUS))
    {
        apontaBola->x -= (apontaBola->x - (WIDTH - CIRCLE_RADIUS));
        apontaBola->dx *= -1;
        return 1;
    }
    return 0;
}

int paredeCima(b_uni *apontaBola)
{
    if (apontaBola->y < BORDA_MINIMA)
    {
        apontaBola->y = CIRCLE_RADIUS;
        apontaBola->dy *= -1;
        return 1;
    }
    return 0;
}

int paredeBaixo(b_uni *apontaBola)
{
    if (apontaBola->y > (HEIGHT - CIRCLE_RADIUS - BORDA_BAIXO))
    {
        apontaBola->y -= apontaBola->y - (HEIGHT - CIRCLE_RADIUS - BORDA_BAIXO);
        apontaBola->dy *= -1;
        return 1;
    }
    return 0;
}

int colisaoBaixo(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    double distancia;
    if ((((apontaBola->x)) >= (j * TAMANHO_QUADRADO)) && (((apontaBola->x)) <= (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO)) && (((apontaBola->y)) >= (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO)))
    {
        distancia = ((apontaBola->y) - (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO));
        if (distancia < (CIRCLE_RADIUS - apontaBola->dy))
        {
            (apontaBola->dy) *= -1;
            apontaBola->y = k * TAMANHO_QUADRADO + TAMANHO_QUADRADO + CIRCLE_RADIUS;
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}
int colisaoDireita(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    double distancia;
    if ((((apontaBola->y)) > (k * TAMANHO_QUADRADO)) && (((apontaBola->y)) < (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO)) && (((apontaBola->x)) >= (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO)))
    {
        distancia = ((apontaBola->x) - (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO));
        if (distancia < (CIRCLE_RADIUS - apontaBola->dx))
        {
            (apontaBola->dx) *= -1;
            apontaBola->x = j * TAMANHO_QUADRADO + TAMANHO_QUADRADO + CIRCLE_RADIUS;
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}

int colisaoCima(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    double distancia;
    if ((((apontaBola->x)) > (j * TAMANHO_QUADRADO)) && (((apontaBola->x)) < (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO)) && (((apontaBola->y)) <= (k * TAMANHO_QUADRADO)))
    {
        distancia = (k * TAMANHO_QUADRADO) - ((apontaBola->y));
        if (distancia < (CIRCLE_RADIUS + apontaBola->dy))
        {
            (apontaBola->dy) *= -1;
            apontaBola->y = k * TAMANHO_QUADRADO - CIRCLE_RADIUS;
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}

int colisaoEsquerda(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    double distancia;
    if ((((apontaBola->y)) > (k * TAMANHO_QUADRADO)) && (((apontaBola->y)) < (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO)) && (((apontaBola->x)) <= (j * TAMANHO_QUADRADO)))
    {
        distancia = (j * TAMANHO_QUADRADO) - ((apontaBola->x));
        if (distancia < (CIRCLE_RADIUS + apontaBola->dx))
        {
            (apontaBola->dx) *= -1;
            apontaBola->x = j * TAMANHO_QUADRADO - CIRCLE_RADIUS;
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}

int verticeInferiorEsquerdo(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    float distancia;
    float distanciaDesloc;
    distanciaDesloc = sqrt(pow((apontaBola->dx), 2) + pow((apontaBola->dy), 2)) / 5;
    if ((((apontaBola->y)) > (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO)) && (((apontaBola->x)) < (j * TAMANHO_QUADRADO)))
    {
        distancia = sqrt(pow(((apontaBola->x)) - (j * TAMANHO_QUADRADO), 2) + pow(((apontaBola->y)) - (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO), 2));
        if (distancia < (CIRCLE_RADIUS + abs(distanciaDesloc)))
        {
            if (((apontaBola->dy)) < 0)
                (apontaBola->dy) *= -(1 + (numAleatFloat(1, 8) / 10));
            if (((apontaBola->dx)) > 0)
                (apontaBola->dx) *= -(1 + (numAleatFloat(1, 8) / 10));
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}

int verticeInferiorDireito(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    float distancia;
    float distanciaDesloc;
    distanciaDesloc = sqrt(pow((apontaBola->dx), 2) + pow((apontaBola->dy), 2)) / 5;
    if ((((apontaBola->y)) > (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO)) && (((apontaBola->x)) > (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO)))
    {
        distancia = sqrt(pow(((apontaBola->x)) - (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO), 2) + pow(((apontaBola->y)) - (k * TAMANHO_QUADRADO + TAMANHO_QUADRADO), 2));
        if (distancia < (CIRCLE_RADIUS + abs(distanciaDesloc)))
        {
            if (((apontaBola->dy)) < 0)
                (apontaBola->dy) *= -(1 + (numAleatFloat(1, 8) / 10));
            if (((apontaBola->dx)) < 0)
                (apontaBola->dx) *= -(1 + (numAleatFloat(1, 8) / 10));
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}

int verticeSuperiorDireito(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    float distancia;
    float distanciaDesloc;
    distanciaDesloc = sqrt(pow((apontaBola->dx), 2) + pow((apontaBola->dy), 2)) / 5;
    if ((((apontaBola->y)) < (k * TAMANHO_QUADRADO)) && (((apontaBola->x)) > (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO)))
    {
        distancia = sqrt(pow(((apontaBola->x)) - (j * TAMANHO_QUADRADO + TAMANHO_QUADRADO), 2) + pow(((apontaBola->y)) - (k * TAMANHO_QUADRADO), 2));
        if (distancia < (CIRCLE_RADIUS + abs(distanciaDesloc)))
        {
            if (((apontaBola->dy)) > 0)
                (apontaBola->dy) *= -(1 + (numAleatFloat(1, 8) / 10));
            if (((apontaBola->dx)) < 0)
                (apontaBola->dx) *= -(1 + (numAleatFloat(1, 8) / 10));
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}
int verticeSuperiorEsquerdo(int j, int k, int linha[QDSALT][QDSLARG], b_uni *apontaBola)
{
    float distancia;
    float distanciaDesloc;
    distanciaDesloc = sqrt(pow((apontaBola->dx), 2) + pow((apontaBola->dy), 2)) / 5;
    if ((((apontaBola->y)) < (k * TAMANHO_QUADRADO)) && (((apontaBola->x)) < (j * TAMANHO_QUADRADO)))
    {
        distancia = sqrt(pow(((apontaBola->x)) - (j * TAMANHO_QUADRADO), 2) + pow(((apontaBola->y)) - (k * TAMANHO_QUADRADO), 2));
        if (distancia < (CIRCLE_RADIUS + abs(distanciaDesloc)))
        {
            if (((apontaBola->dy)) > 0)
                (apontaBola->dy) *= -(1 + (numAleatFloat(1, 8) / 10));
            if (((apontaBola->dx)) > 0)
                (apontaBola->dx) *= -(1 + (numAleatFloat(1, 8) / 10));
            linha[k][j] -= 1;
            return 1;
        }
    }
    return 0;
}

int distribuiColisao(ball *bola, int linha[QDSALT][QDSLARG], inicio *rodadaAtual, bool *done, int *adicionaBola)
{
    ALLEGRO_SAMPLE *collision = al_load_sample("resources/blockcolid.wav");
    must_init(collision, "collision");

    ALLEGRO_SAMPLE *wallcolid = al_load_sample("resources/wlcolid.wav");
    must_init(wallcolid, "wallcolid");

    ALLEGRO_SAMPLE *gainball = al_load_sample("resources/gainball.wav");
    must_init(gainball, "gainball");

    int j, k;
    b_uni *apontaBola;
    double distancia;
    apontaBola = bola->ini;
    int prossegue;
    while (apontaBola != NULL)
    {
        if (apontaBola->ingame)
        {
            apontaBola->x += apontaBola->dx;
            apontaBola->y += apontaBola->dy;

            if (paredeEsquerda(apontaBola))
                al_play_sample(wallcolid, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (paredeDireita(apontaBola))
                al_play_sample(wallcolid, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (paredeCima(apontaBola))
                al_play_sample(wallcolid, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            if (paredeBaixo(apontaBola))
            {
                apontaBola->ingame = 0;
                apontaBola->rodou = 1;
                atualizaRodada(rodadaAtual, apontaBola);
                apontaBola->x = rodadaAtual->x;
                apontaBola->y = rodadaAtual->y;
                prossegue = novoNivel(bola, rodadaAtual, linha, done);
            }
            for (k = 0; k < QDSALT; k++)
            {
                for (j = 0; j < QDSLARG; j++)
                {
                    if ((linha[k][j]) > 0)
                    {
                        if (colisaoBaixo(j, k, linha, apontaBola))
                        {
                            al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        }
                        else if (colisaoDireita(j, k, linha, apontaBola))
                        {
                            al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        }
                        else if (colisaoCima(j, k, linha, apontaBola))
                        {
                            al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        }
                        else if (colisaoEsquerda(j, k, linha, apontaBola))
                        {
                            al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        }
                        else
                        {
                            if (verticeInferiorEsquerdo(j, k, linha, apontaBola))
                            {
                                al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            }
                            else if (verticeInferiorDireito(j, k, linha, apontaBola))
                            {
                                al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            }
                            else if (verticeSuperiorDireito(j, k, linha, apontaBola))
                            {
                                al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            }
                            else if (verticeSuperiorEsquerdo(j, k, linha, apontaBola))
                            {
                                al_play_sample(collision, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                                break;
                            }
                        }
                    }
                    if ((linha[k][j]) == -1)
                    {
                        distancia = sqrt(pow((j * TAMANHO_QUADRADO + MEIO_QUADRADO) - (apontaBola->x), 2) + pow((k * TAMANHO_QUADRADO + MEIO_QUADRADO) - (apontaBola->y), 2));
                        if (distancia < (TAMANHO_QUADRADO / 2 + CIRCLE_RADIUS))
                        {
                            linha[k][j] -= 2;
                            *adicionaBola += 1;
                            al_play_sample(gainball, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                            break;
                        }
                    }
                }
            }
        }
        if (apontaBola->prox == NULL)
            break;
        apontaBola = apontaBola->prox;
    }

    return prossegue;
}