#if !MAP_H
#define MAP_H

#define MAX_X 80
#define MAX_Y 24

typedef int map_t[MAX_X][MAX_Y];

map_t* map_get(void);

void map_print(map_t* map);

int map_is_block(map_t* map, int x, int y);

#endif
