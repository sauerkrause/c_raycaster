#if !MAP_H
#define MAP_H
#include <stdio.h>

#define MAX_X 1024
#define MAX_Y 1024

typedef int map_t[MAX_X][MAX_Y];

map_t* map_get(void);

void map_print(map_t* map);

/* Map serialization format is based on s-expressions as follows: */
/* (MAP 
     '(count 
       '((x y block) 
          ...))) */
void map_serialize(map_t* map, FILE* output, int mode);

int map_is_block(map_t* map, int x, int y);

#endif
