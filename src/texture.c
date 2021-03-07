#include "texture.h"
#include "SDL_dep.h"
#include <string.h>

static void texture_name_add(const char* name, uint8_t id);
static int8_t texture_name_locate(const char* name, uint8_t* id);

int texture_cmp_name(const void* a, const void* b);

int texture_cmp_name(const void * a, const void * b)
{
  const texture_name_t * name_a = a;
  const texture_name_t * name_b = b;
  return strcmp(name_a->name, name_b->name);
}

texture_list_t texture_list()
{
  static texture_list_t list_textures;
  return list_textures;
}

texture_t* texture_load(const char * filename, const char * name)
{
  static uint8_t num_textures = 0;
  SDL_Surface* surface;
  uint8_t return_offset = 0;

  surface = 0;
  surface = IMG_Load(filename);
  if(surface) {
    uint8_t existing_id;
    int8_t exists;
    texture_t* texture;
    exists = texture_name_locate(name, &existing_id);
    if(exists) {
      return_offset = existing_id;
      texture = texture_list() + existing_id;
      SDL_FreeSurface(texture->image);
    } else {
      return_offset = num_textures;
      texture = texture_list() + num_textures;
      texture->id = num_textures;
      texture_name_add(name, num_textures++);
    }
    texture->image = surface; 
  }
  return texture_list() + return_offset;
}

static texture_name_list_t* texture_name_list_get()
{
  static texture_name_list_t name_list;
  return &name_list;
}

static int8_t texture_name_locate(const char* name, uint8_t* id)
{
  texture_name_t* element;
  texture_name_list_t* list;
  list = texture_name_list_get();
  element = bsearch(name, list->names, list->size, sizeof(texture_name_t), &texture_cmp_name);

  if(element) {
    *id = element->id;
    return 1;
  } else {
    return 0;
  }
}

static void texture_name_add(const char* name, uint8_t id)
{
  texture_name_list_t* list;
  list = texture_name_list_get();
  list->names[list->size].name = name;
  list->names[list->size++].id = id;
  qsort(list, list->size, sizeof(texture_name_t), &texture_cmp_name);
}
