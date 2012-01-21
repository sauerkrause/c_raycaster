#default cmdline flags
CC=gcc
CFLAGS=-ansi -Wall -pedantic -O0 -g
COMMON_LDFLAGS = -lm -lncurses

ifeq "$(OSTYPE)" "darwin11"
	LDFLAGS = $(COMMON_LDFLAGS) -framework Cocoa -framework OpenGL -framework GLUT
else
	LDFLAGS = $(COMMON_LDFLAGS) -lGL -lglut -lGLU
endif

all : main.o vec2.o map.o raycaster.o time.o framebuffer.o enemy_director.o demo_state.o threading.o
	${CC} main.o vec2.o map.o raycaster.o time.o framebuffer.o enemy_director.o demo_state.o threading.o $(LDFLAGS) -o test

vec2.o : src/vec2.c src/vec2.h
	${CC} ${CFLAGS} ${SOFLAGS} -c src/vec2.c

main.o : src/main.c
	${CC} ${CFLAGS} ${SOFLAGS} -c src/main.c

map.o : src/map.c src/map.h
	${CC} ${CFLAGS} ${SOFLAGS} -c src/map.c

raycaster.o : src/raycaster.c src/raycaster.h
	${CC} ${CFLAGS} ${SOFLAGS} -c src/raycaster.c

time.o : src/time.h src/time.c
	${CC} ${CFLAGS} ${SOFLAGS} -c src/time.c

demo_state.o : src/demo_state.h src/demo_state.c
	${CC} ${CFLAGS} ${SOFLAGS} -c src/demo_state.c

framebuffer.o : src/framebuffer.h src/framebuffer.c
	${CC} ${CFLAGS} ${SOFLAGS} -c src/framebuffer.c

enemy_director.o : src/enemy_director.h src/enemy_director.c
	${CC} ${CFLAGS} ${SOFLAGS} -c src/enemy_director.c

threading.o : src/threading.h src/threading.c
	${CC} ${CFLAGS} ${SOFLAGS} -c src/threading.c

clean :
	rm ./*.o
