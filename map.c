#include "map.h"
#include <stdlib.h>
#include <stdio.h>

map_t* map_get(void)
{
  static map_t* map = 0;
  if(!map) {
    int i,j;
    map = malloc(sizeof(map_t));
    for(i = 0; i < MAX_X; ++i)
      for(j = 0; j < MAX_Y; ++j) {
	map[0][i][j] = ( i == 0 || i == MAX_X-1 || j == 0 || j == MAX_Y-1 );
      }
  }
  return map;
}

void map_print(map_t* map)
{
  int i, j;
  if(map)
    for(j = MAX_Y-1; j >= 0; --j) {
      for(i = 0; i < MAX_X; ++i)
	printf("%d", *(*( (*map) + i) + j) );
      printf("\n");
    }
}

int map_is_block(map_t* map, int x, int y)
{
  if(x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
    return 0;
  return (*map)[x][y];
}
