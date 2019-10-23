#ifdef __APPLE__
#include "SDL/SDL.h"
#include "SDL/SDL_main.h"
#include "SDL_image.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#endif

#ifdef __APPLE__
#define MAIN SDL_Main
#else
#define MAIN main
#endif
