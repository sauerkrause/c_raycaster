#ifdef __APPLE__
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"
#else
#include "SDL/SDL.h"
#endif

#ifdef __APPLE__
#define MAIN SDL_Main
#else
#define MAIN main
#endif
