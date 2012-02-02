#include "framebuffer.h"
#include "opengl_dep.h"
#include <stdlib.h>
#include <memory.h>
#include "SDL_dep.h"


#define MAKE_RGBA(r,g,b,a) \
  (a & 0xf) << 0 | (b & 0xf) << 4 | (g & 0xf) << 8 | (r & 0xf) << 12

static SDL_Surface* framebuffer = 0;

SDL_Surface * framebuffer_get()
{
  return framebuffer;
}

void framebuffer_set(SDL_Surface* fb)
{
  framebuffer = fb;
}

void framebuffer_wipe()
{
  SDL_PixelFormat *fmt;
  fmt = framebuffer->format;

  memset(framebuffer->pixels,
	 0, framebuffer->w*framebuffer->h*fmt->BytesPerPixel);
}
