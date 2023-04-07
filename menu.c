#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "estruturas.h"
#include "menu.h"

void must_init(bool test, const char *description)
{
    if (test)
        return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int instructions(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, bool *done, bool *redraw, unsigned char *key)
{
    ALLEGRO_BITMAP *instruct = al_load_bitmap("./resources/instructions.png");
    must_init(instruct, "instruct");
    al_flip_display();
    al_wait_for_event(queue, &event);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(instruct, 0, 0, 0);
    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
        if (key[ALLEGRO_KEY_ESCAPE])
            *done = true;
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= KEY_SEEN;
        *redraw = true;
        break;
    case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        if (key[ALLEGRO_KEY_H])
        {
            al_destroy_bitmap(instruct);
            return 0;
        }
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        *done = true;
        break;
    }
    al_destroy_bitmap(instruct);
    return 1;
}

void registraRecord(inicio rodadaAtual)
{
    int valor[NIVEL_MAX];
    FILE *arq;
    int i = 0;
    int j = 0;
    int indice;
    int stop = 0;
    arq = fopen("./resources/records.txt", "r");
    if (!arq)
    {
        perror("Erro ao abrir o arquivo!");
        exit(1);
    }
    while (!feof(arq))
    {
        fscanf(arq, "%d", &valor[i]);
        i++;
    }
    fclose(arq);
    arq = fopen("./resources/records.txt", "w");
    if (!arq)
    {
        perror("Erro ao abrir o arquivo!");
        exit(1);
    }
    valor[(i - 1)] = rodadaAtual.nivel;
    for (j = 0; j < i; j++)
    {
        if (rodadaAtual.nivel > (valor[j]))
        {
            indice = j;
            for (j = (i - 1); j >= indice; j--)
            {
                valor[j] = valor[j - 1];
                stop = 1;
            }
            valor[indice] = rodadaAtual.nivel;
        }
        if (stop)
            break;
    }

    for (j = 0; j < i; j++)
        fprintf(arq, "%d\n", valor[j]);
    fclose(arq);
}

int telaFim(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_MOUSE_STATE state, bool *done, bool *redraw, unsigned char *key, inicio rodadaAtual, int *mostraMenu)
{
    ALLEGRO_FONT *limefont = al_load_font("./resources/LimeDays.ttf", 36, 0);
    FILE *arq;
    int i = 0;
    char linha[MAX_PALAVRA];
    int valor;
    int noRepeat = 1;
    ALLEGRO_BITMAP *playagain = al_load_bitmap("./resources/playagain.png");
    must_init(playagain, "playagain");
    ALLEGRO_BITMAP *mainmenu = al_load_bitmap("./resources/mainmenu.png");
    must_init(mainmenu, "mainmenu");

    arq = fopen("./resources/records.txt", "r");
    if (!arq)
    {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    al_get_mouse_state(&state);
    al_wait_for_event(queue, &event);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_textf(limefont, al_map_rgb(255, 255, 255), WIDTH / 2, TAMANHO_QUADRADO, ALLEGRO_ALIGN_CENTER, "Records");
    al_draw_bitmap(mainmenu, WIDTH / 3, (HEIGHT - TAMANHO_QUADRADO - 75), 0);
    al_draw_bitmap(playagain, WIDTH / 3, (HEIGHT - (TAMANHO_QUADRADO * 3) - 25), 0);
    rewind(arq);
    while ((!feof(arq)) && (i < 10))
    {
        if (fgets(linha, MAX_PALAVRA, arq))
        {
            sscanf(linha, "%d", &valor);
            if ((valor == rodadaAtual.nivel) && (noRepeat))
            {
                al_draw_textf(limefont, al_map_rgb(255, 255, 255), WIDTH / 2, (TAMANHO_QUADRADO + (TAMANHO_QUADRADO / 2) * (i + 1)), ALLEGRO_ALIGN_CENTER, "You   .............................. %5d", valor);
                noRepeat = 0;
            }
            else
                al_draw_textf(limefont, al_map_rgb(255, 255, 255), WIDTH / 2, (TAMANHO_QUADRADO + (TAMANHO_QUADRADO / 2) * (i + 1)), ALLEGRO_ALIGN_CENTER, "%2d     .............................. %5d", (i + 1), valor);
            i++;
        }
    }
    if (noRepeat)
        al_draw_textf(limefont, al_map_rgb(255, 255, 255), WIDTH / 2, (TAMANHO_QUADRADO + (TAMANHO_QUADRADO / 2) * (i + 1)), ALLEGRO_ALIGN_CENTER, "You   ............................. %5d", rodadaAtual.nivel);

    *done = false;

    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
        if (key[ALLEGRO_KEY_ESCAPE])
            *done = true;

        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= KEY_SEEN;

        *redraw = true;
        break;
    case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        *done = true;
        break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        al_get_mouse_state(&state);
        if (((state.x) >= (WIDTH / 3)) && ((state.x) <= (WIDTH / 3 + WIDTH / 3)))
        {
            if ((state.y >= (HEIGHT - (TAMANHO_QUADRADO * 3) - 25)) && ((state.y) <= (HEIGHT - (TAMANHO_QUADRADO * 3) - 25 + 100)))
            {
                if (!(state.buttons & 1))
                {
                    *mostraMenu = 0;
                    return 0;
                }
            }
            if ((state.y >= (HEIGHT - TAMANHO_QUADRADO - 75)) && ((state.y) <= (HEIGHT - TAMANHO_QUADRADO - 75 + 100)))
            {
                if (!(state.buttons & 1))
                {
                    *mostraMenu = 1;
                    return 0;
                }
            }
        }
        break;
    }
    fclose(arq);
    al_destroy_font(limefont);
    al_destroy_bitmap(playagain);
    al_destroy_bitmap(mainmenu);
    return 1;
}

int menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_MOUSE_STATE state, bool *done, bool *redraw, unsigned char *key)
{
    ALLEGRO_FONT *biglimefont = al_load_font("./resources/LimeDays.ttf", 72, 0);
    ALLEGRO_BITMAP *play = al_load_bitmap("./resources/play.png");
    must_init(play, "play");
    ALLEGRO_BITMAP *nuvem = al_load_bitmap("./resources/nuvem.png");
    must_init(nuvem, "nuvem");
    al_get_mouse_state(&state);
    al_wait_for_event(queue, &event);
    al_flip_display();
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rounded_rectangle(WIDTH / 4, 600, (WIDTH - WIDTH / 4), 800, 20, 20, al_map_rgb(136, 206, 235));
    al_draw_textf(biglimefont, al_map_rgb(0, 0, 100), WIDTH / 2, 655, ALLEGRO_ALIGN_CENTER, "PLAY");
    al_draw_bitmap(nuvem, 40, 100, 0);
    al_draw_text(biglimefont, al_map_rgb(30, 144, 255), WIDTH / 2 - 70, 230, ALLEGRO_ALIGN_CENTER, "SKY");
    al_draw_text(biglimefont, al_map_rgb(0, 0, 139), WIDTH / 2 + 70, 230, ALLEGRO_ALIGN_CENTER, "BALL");
    switch (event.type)
    {
    case ALLEGRO_EVENT_TIMER:
        if (key[ALLEGRO_KEY_ESCAPE])
            *done = true;
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
            key[i] &= KEY_SEEN;
        *redraw = true;
        break;
    case ALLEGRO_EVENT_KEY_DOWN:
        key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
        break;
    case ALLEGRO_EVENT_KEY_UP:
        if (key[ALLEGRO_KEY_H])
        {
            while (1)
            {
                if (!(instructions(queue, event, done, redraw, key)))
                    break;
            }
        }
        key[event.keyboard.keycode] &= KEY_RELEASED;
        break;

    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        *done = true;
        break;

    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        al_get_mouse_state(&state);
        if (((state.x) >= (WIDTH / 4)) && ((state.x) <= (WIDTH - WIDTH / 4)))
        {
            if ((state.y >= 600) && ((state.y) <= 800))
            {
                if (!(state.buttons & 1))
                {
                    al_destroy_bitmap(play);
                    al_destroy_bitmap(nuvem);
                    al_destroy_font(biglimefont);
                    return 0;
                }
            }
        }
        break;
    }
    al_destroy_bitmap(play);
    al_destroy_bitmap(nuvem);
    al_destroy_font(biglimefont);
    return 1;
}