/*

                                        Trabalho 2 - Programação 2
        SkyBall - Desenvolvimento do jogo Ballz em linguagem C, utilizando a biblioteca Allegro de auxiliar
                            Aluno: Leonardo Becker de Oliveira - GRR 20211779
*/  

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include "estruturas.h"
#include "ingame.h"
#include "bola.h"
#include "colisao.h"
#include "menu.h"

int main()
{
    int prossegue = 0;
    inicio rodadaAtual;
    int linha[QDSALT][QDSLARG];
    double selecX = 0;
    double selecY = 0;
    ball *bola;
    int mostraMenu = 1;
    int adicionaBola=0;

    srand(time(NULL));

    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    must_init(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY *disp = al_create_display(WIDTH, HEIGHT);
    must_init(disp, "display");

    ALLEGRO_FONT *font = al_create_builtin_font();
    must_init(font, "font");

    al_init_font_addon();
    al_init_ttf_addon();

    must_init(al_init_image_addon(), "image addon");

    must_init(al_init_primitives_addon(), "primitives");

    must_init(al_install_mouse(), "mouse");

    must_init(al_install_audio(), "audio");
    must_init(al_init_acodec_addon(), "audio codecs");
    must_init(al_reserve_samples(16), "reserve samples");

    ALLEGRO_SAMPLE *easteregg = al_load_sample("resources/easteregg.wav");
    must_init(easteregg, "easteregg");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_mouse_event_source());

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    ALLEGRO_MOUSE_STATE state;
    al_grab_mouse(disp);

    unsigned char key[ALLEGRO_KEY_MAX];
    memset(key, 0, sizeof(key));

    al_start_timer(timer);

    while (1)
    {
        if (mostraMenu)
        {
            while (menu(queue, event, state, &done, &redraw, key))
            {
                al_show_mouse_cursor(disp);
                if (done)
                    return 0;
            }
        }

        bola = bolaCria();
        inicializaValores(&rodadaAtual, linha);
        adicionaBola=0;
        bolaInsere(bola, rodadaAtual);
        done = false;
        redraw = true;

        /* Laço referente a parte principal do jogo */
        while (1)
        {
            al_hide_mouse_cursor(disp);
            al_wait_for_event(queue, &event);
            switch (event.type)
            {
            case ALLEGRO_EVENT_TIMER:
                if (key[ALLEGRO_KEY_ESCAPE])
                    done = true;
                for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
                    key[i] &= KEY_SEEN;
                if (prossegue)
                {
                    prossegue = distribuiColisao(bola, linha, &rodadaAtual, &done, &adicionaBola);
                }
                redraw = true;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event.keyboard.keycode] = KEY_SEEN | KEY_RELEASED;
                break;
            case ALLEGRO_EVENT_KEY_UP:
                if (key[ALLEGRO_KEY_F])
                    aceleraBolas(bola);
                if ((key[ALLEGRO_KEY_ALT]) && (key[ALLEGRO_KEY_F4]))
                {
                    easterEgg(linha);
                    al_play_sample(easteregg, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                key[event.keyboard.keycode] &= KEY_RELEASED;
                break;
            }
            if (done)
            {
                registraRecord(rodadaAtual);
                while (telaFim(queue, event, state, &done, &redraw, key, rodadaAtual, &mostraMenu))
                {
                    al_show_mouse_cursor(disp);
                    if (done)
                        break;
                }
                break;
            }
            emJogo(&redraw, queue, &rodadaAtual, &prossegue, bola, &state, &selecX, &selecY, linha, &adicionaBola);
        }
        if (done)
            break;
        bolaDestroi(bola);
    }
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_sample(easteregg);

    return 0;
}
