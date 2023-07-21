#default cmdline flags
CC=gcc
CFLAGS=-ansi -Wall -pedantic -O0 -g

COMMON_LDFLAGS = -lm -lncurses

ifeq "$(OSTYPE)" "darwin11"
	LDFLAGS = $(COMMON_LDFLAGS) -framework Cocoa -framework SDL -framework SDL_image -lSDLmain
	SDL_INCLUDES = -I/Library/Frameworks/SDL.framework/Headers/ -I/Library/Frameworks/SDL_image.framework/Headers/
else
	LDFLAGS = $(COMMON_LDFLAGS) -lSDL -lSDL_image
	SDL_INCLUDES = 
endif

INCLUDES = $(SDL_INCLUDES) -Iinclude/

all : main.o vec2.o map.o raycaster.o time.o framebuffer.o enemy_director.o demo_state.o xorshift.o texture.o renderer.o controller.o game.o span.o
	${CC} main.o game.o span.o vec2.o map.o raycaster.o time.o framebuffer.o enemy_director.o demo_state.o xorshift.o texture.o renderer.o controller.o $(LDFLAGS) -o test

controller.o : src/controller.c include/controller.h
	${CC} ${CFLAGS} ${INCLUDES} -c src/controller.c

renderer.o : src/renderer.c include/renderer.h
	${CC} ${CFLAGS} ${INCLUDES} -c src/renderer.c

vec2.o : src/vec2.c include/vec2.h
	${CC} ${CFLAGS} ${INCLUDES} -c src/vec2.c

main.o : src/main.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/main.c

map.o : src/map.c include/map.h
	${CC} ${CFLAGS} ${INCLUDES} -c src/map.c

game.o : src/game.c include/game.h
	${CC} ${CFLAGS} ${INCLUDES} -c src/game.c

raycaster.o : src/raycaster.c include/raycaster.h
	${CC} ${CFLAGS} ${INCLUDES} -c src/raycaster.c

time.o : include/time.h src/time.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/time.c

demo_state.o : include/demo_state.h src/demo_state.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/demo_state.c

framebuffer.o : include/framebuffer.h src/framebuffer.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/framebuffer.c

enemy_director.o : include/enemy_director.h src/enemy_director.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/enemy_director.c

xorshift.o : include/xorshift.h src/xorshift.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/xorshift.c

texture.o : include/texture.h src/texture.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/texture.c

span.o : src/span.c
	${CC} ${CFLAGS} ${INCLUDES} -c src/span.c

clean :
	rm ./*.o
