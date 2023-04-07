#ifndef _Menu_
#define _Menu_

/* Testa se os objetos da biblioteca allegro foram iniciados corretamente */
void must_init(bool test, const char *description);

/* Função responsável por apresentar a tela de instruções quando pressionado a tecla predefinida */
int instructions(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, bool *done, bool *redraw, unsigned char *key);

/* Função responsável por preencher o arquivo de texto, mantendo assim as pontuações obtida pelos jogadores */
void registraRecord(inicio rodadaAtual);

/* Função responsável por finalizar o jogo, mostrando assim os records e outras opções uteis */
int telaFim(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_MOUSE_STATE state, bool *done, bool *redraw, unsigned char *key, inicio rodadaAtual, int *mostraMenu);

/* Primeira tela apresentada no jogo, podendo a partir tela, iniciar a jogabilidade */
int menu(ALLEGRO_EVENT_QUEUE *queue, ALLEGRO_EVENT event, ALLEGRO_MOUSE_STATE state, bool *done, bool *redraw, unsigned char *key);

#endif