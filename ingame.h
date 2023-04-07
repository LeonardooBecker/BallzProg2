#ifndef _InGame_
#define _InGame_

/* Gera números aleatorios entre os paramentros de inicio e fim */
int numAleat(int inicio, int fim);

/* Acerela a velocidade das bolas, durante a partida */
void aceleraBolas(ball *bola);

/* Função responsável por gerar as novas linhas do jogo, prosseguindo assim com a jogabilidade */
int novoNivel(ball *bola, inicio *rodadaAtual, int linha[QDSALT][QDSLARG], bool *done);

/* Função auxiliar para guardar as coordenadas da bola, para poder iniciar o outro nivel corretamente */
void atualizaRodada(inicio *rodadaAtual, b_uni *apontaBola);

/* Função responsável por inicializar o jogo, com os parametros padrão de inicio */
void inicializaValores(inicio *rodadaAtual, int linha[QDSALT][QDSLARG]);

/* Função responsável por limpar toda a tela, quando a tecla do easterEgg é pressionada */
void easterEgg(int linha[QDSALT][QDSLARG]);

/* Função principal do jogo, reponsável por toda a jogabilidade, chamando outras funções de acordo com a localização das bolas */
void emJogo(bool *redraw, ALLEGRO_EVENT_QUEUE *queue, inicio *rodadaAtual, int *prossegue, ball *bola, ALLEGRO_MOUSE_STATE *state, double *selecX, double *selecY, int linha[QDSALT][QDSLARG], int *adicionaBola);

#endif