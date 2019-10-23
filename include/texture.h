#ifndef TEXTURE_H
#define TEXTURE_H

#include "SDL_dep.h"

struct texture_s {
  uint8_t id;
  SDL_Surface* image;
};

typedef struct texture_s texture_t;

typedef texture_t *texture_list_t;

typedef struct texture_name_s {
  const char* name;
  uint8_t id;
} texture_name_t;

texture_list_t texture_list();

texture_t* texture_load(const char * filename, const char * name);

typedef struct texture_name_list_s {
  texture_name_t names[255];
  uint8_t size;
} texture_name_list_t;

#endif
