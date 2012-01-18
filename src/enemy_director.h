#ifndef ENEMY_DIRECTOR_H
#define ENEMY_DIRECTOR_H

#include "vec2.h"

typedef struct enemy_t {
  vec2_t position;
  float width;
  long id;
  unsigned char valid;
} enemy_t;

typedef struct long_free_list_t
{
  long data;
  struct long_free_list_t* next;
} long_free_list_t;

typedef struct enemy_director_t {
  enemy_t* enemies;
  long size;
  long _max_size;
  long_free_list_t* first_free;
} enemy_director_t;

void enemy_director_init(enemy_director_t* dir);
void enemy_director_add(enemy_director_t* dir,  enemy_t* enemy);
void enemy_director_del(enemy_director_t* dir, long id);
void enemy_director_free(enemy_director_t* dir);
#endif
