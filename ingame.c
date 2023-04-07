#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "estruturas.h"
#include "bola.h"
#include "menu.h"
#include "ingame.h"

int numAleat(int inicio, int fim)
{
    return (rand() % (fim - inicio + 1) + inicio);
}

void aceleraBolas(ball *bola)
{
    b_uni *apontaBola;
    apontaBola = bola->ini;
    while (apontaBola != NULL)
    {
        if ((apontaBola->ingame) && ((bola->fim->ingame) || (bola->fim->rodou)))
        {
            while ((abs(apontaBola->dx) <= 23) && (abs(apontaBola->dy) <= 23))
            {
                apontaBola->dx *= 1.3;
                apontaBola->dy *= 1.3;
            }
        }
        if (apontaBola->prox == NULL)
            break;
        apontaBola = apontaBola->prox;
    }
}

int novoNivel(ball *bola, inicio *rodadaAtual, int linha[QDSALT][QDSLARG], bool *done)
{
    ALLEGRO_SAMPLE *endnivel = al_load_sample("resources/endnivel.wav");
    must_init(endnivel, "endnivel");

    ALLEGRO_SAMPLE *endgame = al_load_sample("resources/endgame.wav");
    must_init(endgame, "endgame");

    b_uni *ptrTestaFim;
    ptrTestaFim = bola->ini;
    int i;
    int j;

    while (ptrTestaFim != NULL)
    {
        if ((ptrTestaFim->ingame))
        {
            al_destroy_sample(endnivel);
            al_destroy_sample(endgame);
            return 1;
        }
        if ((ptrTestaFim == bola->fim))
        {
            for (i = 0; i < QDSLARG; i++)
            {
                if (numAleat(0, 1))
                {
                    if ((rodadaAtual->nivel) < 6)
                        linha[0][i] = numAleat(0, (rodadaAtual->nivel));
                    else
                    {
                        linha[0][i] = numAleat(((rodadaAtual->nivel) - 5), (rodadaAtual->nivel));
                        if (numAleat(0, 5) == 2)
                        {
                            linha[0][numAleat(0, QDSLARG)] = numAleat((rodadaAtual->nivel + 15), (rodadaAtual->nivel + 30));
                        }
                    }
                }
            }
            for (i = (QDSALT); i > 0; i--)
            {
                for (j = 0; j < QDSLARG; j++)
                {
                    al_play_sample(endnivel, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                    linha[i][j] = linha[i - 1][j];
                    linha[i - 1][j] = 0;
                    if ((i == QDSALT) && (linha[i][j] > 0))
                    {
                        al_play_sample(endgame, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                        *done = true;
                    }
                }
            }
            linha[1][(numAleat(0, (QDSLARG - 1)))] = -1;
            rodadaAtual->nivel += 1;
        }

        if (ptrTestaFim->prox == NULL)
            break;
        ptrTestaFim = ptrTestaFim->prox;
    }
    return 0;
}

void atualizaRodada(inicio *rodadaAtual, b_uni *apontaBola)
{
    if ((rodadaAtual->atualiza) == 1)
    {
        rodadaAtual->x = apontaBola->x;
        rodadaAtual->y = apontaBola->y;
        rodadaAtual->atualiza = 0;
    }
}

void inicializaValores(inicio *rodadaAtual, int linha[QDSALT][QDSLARG])
{
    int i, j;
    rodadaAtual->x = WIDTH / 2;
    rodadaAtual->y = HEIGHT - BORDA_MINIMA - BORDA_BAIXO;
    rodadaAtual->atualiza = 1;
    rodadaAtual->nivel = 1;

    for (i = 0; i < QDSALT; i++)
    {
        for (j = 0; j < QDSLARG; j++)
        {
            if (i == 1)
                linha[i][j] = numAleat(0, 1);
            else
                linha[i][j] = 0;
        }
    }
}

void easterEgg(int linha[QDSALT][QDSLARG])
{
    int i, j;
    for (i = 0; i < QDSALT; i++)
    {
        for (j = 0; j < QDSLARG; j++)
        {
            if (linha[i][j] > 0)
                linha[i][j] = 0;
        }
    }
}

void emJogo(bool *redraw, ALLEGRO_EVENT_QUEUE *queue, inicio *rodadaAtual, int *prossegue, ball *bola, ALLEGRO_MOUSE_STATE *state, double *selecX, double *selecY, int linha[QDSALT][QDSLARG], int *adicionaBola)
{
    ALLEGRO_BITMAP *lua = al_load_bitmap("./resources/lua.png");
    ALLEGRO_FONT *limefont = al_load_font("./resources/LimeDays.ttf", 36, 0);
    ALLEGRO_FONT *font = al_create_builtin_font();
    must_init(font, "font");

    b_uni *apontaBola;
    double distancia;
    int i, j;
    if ((*redraw) && al_is_event_queue_empty(queue))
    {
        al_get_mouse_state(state);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(0, (HEIGHT - BORDA_BAIXO), WIDTH, HEIGHT, al_map_rgb(50, 50, 50));
        al_draw_textf(limefont, al_map_rgb(255, 255, 255), WIDTH / 2, (HEIGHT - 50), ALLEGRO_ALIGN_CENTRE, "%d", rodadaAtual->nivel);
        if ((*prossegue) == 0)
        {
            if ((*adicionaBola) > 0)
            {
                bolaInsere(bola, *rodadaAtual);
                (*adicionaBola) -= 1;
            }
            al_draw_textf(font, al_map_rgb(255, 255, 255), rodadaAtual->x + 30, rodadaAtual->y - 30, ALLEGRO_ALIGN_RIGHT, "x%d", bola->qntBolas);
            apontaBola = bola->ini;
            while (apontaBola != NULL)
            {
                apontaBola->x = rodadaAtual->x;
                apontaBola->y = rodadaAtual->y;
                if (apontaBola->prox == NULL)
                    break;
                apontaBola = apontaBola->prox;
            }

            if ((state->buttons & 1))
            {
                if (state->y < (HEIGHT - BORDA_BAIXO - 2 * CIRCLE_RADIUS))
                {
                    (*selecX) = state->x;
                    (*selecY) = state->y;
                    apontaBola = bola->ini;
                    apontaBola->dx = ((*selecX) - apontaBola->x) / 40;
                    apontaBola->dy = ((*selecY) - apontaBola->y) / 40;
                    apontaBola->ingame = 1;
                    rodadaAtual->atualiza = 1;
                    (*prossegue) = 1;
                    while (apontaBola != NULL)
                    {
                        apontaBola->rodou = 0;
                        if (apontaBola->prox == NULL)
                            break;
                        apontaBola = apontaBola->prox;
                    }
                }
            }
        }

        if (((bola->fim->ingame) == 0) && ((*prossegue)))
        {
            apontaBola = bola->ini;
            while (apontaBola->prox != NULL)
            {
                if (((apontaBola->prox->ingame) == 0) && (apontaBola->prox->rodou == 0))
                {
                    distancia = sqrt(pow((apontaBola->x) - (apontaBola->prox->x), 2) + pow((apontaBola->y) - (apontaBola->prox->y), 2));
                    if (distancia > (CIRCLE_RADIUS * 5))
                    {
                        apontaBola->prox->dx = ((*selecX) - apontaBola->prox->x) / 40;
                        apontaBola->prox->dy = ((*selecY) - apontaBola->prox->y) / 40;
                        apontaBola->prox->ingame = 1;
                        al_draw_textf(font, al_map_rgb(255, 255, 255), bola->fim->x + 30, bola->fim->y - 30, ALLEGRO_ALIGN_RIGHT, "x%d", (bola->qntBolas - (apontaBola->elemento) - 1));
                        break;
                    }
                }
                apontaBola = apontaBola->prox;
            }
        }
        for (i = 0; i < QDSALT; i++)
        {
            for (j = 0; j < QDSLARG; j++)
            {
                if ((linha[i][j]) > 0)
                {
                    al_draw_filled_rectangle(j * TAMANHO_QUADRADO + BORDA_QUADRADO, i * TAMANHO_QUADRADO + BORDA_QUADRADO, j * TAMANHO_QUADRADO + TAMANHO_QUADRADO - BORDA_QUADRADO, i * TAMANHO_QUADRADO + TAMANHO_QUADRADO - BORDA_QUADRADO, al_map_rgb(linha[i][j], ((linha[i][j] + (255 - linha[i][j])) / 2), (255 - linha[i][j])));
                    al_draw_textf(limefont, al_map_rgb(0, 0, 0), j * TAMANHO_QUADRADO + (TAMANHO_QUADRADO / 2), i * TAMANHO_QUADRADO + (TAMANHO_QUADRADO / 4), ALLEGRO_ALIGN_CENTRE, "%d", linha[i][j]);
                }
                if ((linha[i][j]) == -1)
                {
                    al_draw_bitmap(lua, (j * TAMANHO_QUADRADO), (i * TAMANHO_QUADRADO), 0);
                }
            }
        }
        apontaBola = bola->ini;
        while (apontaBola != NULL)
        {
            if ((*prossegue) == 0)
            {
                if (state->y < (HEIGHT - BORDA_BAIXO - 2 * CIRCLE_RADIUS))
                    al_draw_line(apontaBola->x, apontaBola->y, state->x, state->y, al_map_rgb(0, 255, 255), 3);
            }
            if (((bola->fim->ingame) || (bola->fim->rodou)) && ((*prossegue)))
                al_draw_textf(font, al_map_rgb(160, 160, 160), WIDTH, 0, ALLEGRO_ALIGN_RIGHT, "Pressione F para acelerar");
            al_draw_filled_circle(apontaBola->x, apontaBola->y, CIRCLE_RADIUS, al_map_rgb(255, 255, 255));
            if (apontaBola->prox == NULL)
                break;
            apontaBola = apontaBola->prox;
        }

        al_flip_display();

        (*redraw) = false;
    }
    al_destroy_bitmap(lua);
    al_destroy_font(font);
    al_destroy_font(limefont);
}