#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "SDL_dep.h"

/* retrieves the cpu-side framebuffer */
SDL_Surface * framebuffer_get(); 

/* sets the cpu-side framebuffer */
void framebuffer_set(SDL_Surface * fb);

/* wipes framebuffer clean */
void framebuffer_wipe();

#endif
