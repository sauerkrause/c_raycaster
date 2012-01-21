#default cmdline flags
CC=clang
CFLAGS=-ansi -Wall -pedantic -O2
COMMON_LDFLAGS = -lm -lncurses
SDL_INCLUDES = -I/Library/Frameworks/SDL.framework/Headers/

ifeq "$(OSTYPE)" "darwin11"
	LDFLAGS = $(COMMON_LDFLAGS) -framework Cocoa -framework OpenGL -framework GLUT -framework SDL -lSDLmain
else
	LDFLAGS = $(COMMON_LDFLAGS) -lGL -lglut -lGLU -lSDL
endif

all : main.o vec2.o map.o raycaster.o time.o demo_state.o framebuffer.o
	${CC} main.o map.o vec2.o raycaster.o time.o demo_state.o \
framebuffer.o $(LDFLAGS) -o test

vec2.o : src/vec2.c src/vec2.h
	${CC} ${CFLAGS} -c src/vec2.c

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

clean :
	rm ./*.o