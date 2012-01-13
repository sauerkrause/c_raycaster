#include "enemy_director.h"
#include "vec2.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

/* typedef struct enemy_t { */
/*   vec2_t position; */
/*   float width; */
/* } enemy_t; */

/* typedef struct long_free_list_t */
/* { */
/*   long data; */
/*   long_free_list_t* next; */
/* } long_free_list_t; */

/* typedef struct enemy_director_t { */
/*   enemy_t* enemies; */
/*   long_free_list_t* first_free; */
/* } enemy_director_t; */

static void enemy_director_grow(enemy_director_t* dir);
static void delete_enemy(enemy_director_t* dir, enemy_t* enemy);

void enemy_director_init(enemy_director_t* dir)
{
  dir->enemies = malloc(1 * sizeof(enemy_t));
  dir->first_free = malloc(1 * sizeof(long_free_list_t));
  assert(dir->first_free);
  dir->first_free->next = 0;
  dir->first_free->data = 0;
  dir->size = 0;
  dir->_max_size = 1;
}

void enemy_director_add(enemy_director_t* dir,  enemy_t* enemy)
{
  long_free_list_t* old_free;
  if(dir->first_free == 0)
    enemy_director_grow(dir);
  
  old_free = dir->first_free;
  dir->first_free = old_free->next;
  enemy->id = old_free->data;
  memcpy(dir->enemies + old_free->data, enemy, sizeof(enemy_t));
  free(old_free);
}
static void delete_enemy(enemy_director_t* dir, enemy_t* enemy);
void enemy_director_del(enemy_director_t* dir, long id)
{
  delete_enemy(dir, dir->enemies + id);
}

void delete_enemy(enemy_director_t* dir, enemy_t* enemy)
{
  long_free_list_t* new_free;
  new_free = malloc(sizeof(long_free_list_t));
  new_free->next = dir->first_free;
  new_free->data = enemy->id;
  dir->first_free = new_free; 
  dir->size--;
}

static void enemy_director_grow(enemy_director_t* dir)
{
  int i;
  int error;
  enemy_t* new_enemies;
  i = dir->_max_size;
  dir->_max_size = i * 2;
  error = 0;
  new_enemies = 0;
  new_enemies = realloc(dir->enemies, dir->_max_size * sizeof(enemy_t));

  if(!new_enemies)
    error /= (long)new_enemies;
  dir->enemies = new_enemies;

  for(i = i * 2 - 1; i >= dir->size; --i) {
    long_free_list_t* first_free = malloc(sizeof(long_free_list_t));
    first_free->data = i;
    first_free->next = dir->first_free;
    dir->first_free = first_free;
  }
}

void enemy_director_free(enemy_director_t* dir)
{
  long_free_list_t* free_list;
  free_list = dir->first_free;
  while(free_list) {
    long_free_list_t* current;
    current = free_list;
    free_list = free_list->next;
    free(current);
  }
  free(dir->enemies);
}
