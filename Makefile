#default cmdline flags
CC=gcc
CFLAGS=-ansi -Wall -pedantic -O2

COMMON_LDFLAGS = -lm -lncurses

ifeq "$(OSTYPE)" "darwin11"
	LDFLAGS = $(COMMON_LDFLAGS) -framework Cocoa -framework OpenGL -framework GLUT -framework SDL -lSDLmain
	SDL_INCLUDES = -I/Library/Frameworks/SDL.framework/Headers/
else
	LDFLAGS = $(COMMON_LDFLAGS) -lGL -lglut -lGLU -lSDL
	SDL_INCLUDES = 
endif

all : main.o vec2.o map.o raycaster.o time.o framebuffer.o enemy_director.o demo_state.o
	${CC} main.o vec2.o map.o raycaster.o time.o framebuffer.o enemy_director.o demo_state.o $(LDFLAGS) -o test

vec2.o : src/vec2.c src/vec2.h
	${CC} ${CFLAGS}  -c src/vec2.c

main.o : src/main.c
	${CC} ${SDL_INCLUDES} ${CFLAGS} -c src/main.c

map.o : src/map.c src/map.h
	${CC} ${CFLAGS} -c src/map.c

raycaster.o : src/raycaster.c src/raycaster.h
	${CC} ${CFLAGS} -c src/raycaster.c

time.o : src/time.h src/time.c
	${CC} ${CFLAGS} -c src/time.c

demo_state.o : src/demo_state.h src/demo_state.c
	${CC} ${CFLAGS} -c src/demo_state.c

framebuffer.o : src/framebuffer.h src/framebuffer.c
	${CC} ${CFLAGS} -c src/framebuffer.c

enemy_director.o : src/enemy_director.h src/enemy_director.c
	${CC} ${CFLAGS} -c src/enemy_director.c

clean :
	rm ./*.o
