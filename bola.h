#ifndef _Bola_
#define _Bola_

/* Cria a lista de bolas, com parametros para a proxima bola e a bola final */
ball *bolaCria();

/* Retorna 1 caso nao tenha nenhuma bola em jogo */
int zeroBolas(ball *bola);

/* Insere uma bola na última posição, mantendo os parametros com o decorrer do jogo */
void bolaInsere(ball *bola, inicio rodadaAtual);

/* Destroi a lista de bolas criada */
void bolaDestroi(ball *bola);

#endif