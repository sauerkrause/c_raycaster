#default cmdline flags
CC=clang
CFLAGS=-ansi -Wall -pedantic -O2
COMMON_LDFLAGS = -lm

ifeq "$(OSTYPE)" "darwin11"
	LDFLAGS = $(COMMON_LDFLAGS) -framework Cocoa -framework OpenGL -framework GLUT
else
	LDFLAGS = $(COMMON_LDFLAGS) -lGL -lglut -lGLU
endif

all : main.o vec2.o map.o raycaster.o time.o demo_state.o
	${CC} main.o map.o vec2.o raycaster.o time.o demo_state.o $(LDFLAGS) -o test

vec2.o : vec2.c vec2.h
	${CC} ${CFLAGS} -c vec2.c

main.o : main.c
	${CC} ${CFLAGS} -c main.c

map.o : map.c map.h
	${CC} ${CFLAGS} -c map.c

raycaster.o : raycaster.c raycaster.h
	${CC} ${CFLAGS} -c raycaster.c

time.o : time.h time.c
	${CC} ${CFLAGS} -c time.c

demo_state.o : demo_state.h demo_state.c
	${CC} ${CFLAGS} -c demo_state.c

clean :
	rm ./*.o