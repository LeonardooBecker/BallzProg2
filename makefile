nomePrograma=main

objs=main.o ingame.o menu.o bola.o colisao.o

ALLEGRO_LIBS := allegro-5 \
	allegro_main-5 \
	allegro_image-5 \
	allegro_font-5 \
	allegro_audio-5 \
	allegro_acodec-5 \
	allegro_primitives-5 \
	allegro_ttf-5

CFLAGS := -Wall -std=c99 $$(pkg-config $(ALLEGRO_LIBS) --libs --cflags) -lm

all: $(nomePrograma)

$(nomePrograma): $(objs)
	gcc -o $(nomePrograma) $(objs) $(CFLAGS)

main.o: main.c
	gcc -c -D_DEFAULT_SOURCE main.c $(CFLAGS)

ingame.o: ingame.h ingame.c
	gcc -c ingame.c $(CFLAGS)

menu.o: menu.h menu.c
	gcc -c menu.c $(CFLAGS)

bola.o: bola.h bola.c
	gcc -c bola.c $(CFLAGS)

colidaux.o: colisao.h colisao.c
	gcc -c colisao.c $(CFLAGS)

clean:
	rm -f $(objs) *~

purge: clean
	-rm -f $(nomePrograma)


