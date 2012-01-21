#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void map_serialize(map_t* map, FILE* file, int mode)
{
  int i, j;
  
  if(map) {
    if(mode == 0) {
      /* 0 for input */  
      char line[1024];
      int size;
      size = 0;
      while(fgets(line, 1024, file)) {
	if(!strcmp(line, "(MAP\n")) {
	  fscanf(file, "  (%d", &size);
	  /* printf("%d\n", size); */
	  break;
	}
      }
      while(size--) {
	int x, y, value;
	int scanned_value;
	char* is_not_null;
	x = y = 0;
	value = 1;
	is_not_null = fgets(line, 1024, file);
	scanned_value = is_not_null ? sscanf(line, "    (%d,%d,%d)", &x,&y,&value) : EOF;
	/* printf("%s\n", line); */
	/* printf("%d\n", scanned_value); */
	if( scanned_value == EOF || x < 0) {
	  /* break; */
	}
	*(*(*map + x) + y) = value;
      }
      while(fgets(line, 1024, file)) {
	if(!strcmp(line, "))\n")) {
	  break;
	}
      }
    } else {
      int size;
      size = 0;
      fprintf(file, "(MAP\n");
      for(j = MAX_Y-1; j >= 0; --j)
	for(i = 0; i < MAX_X; ++i)
	  if(*(*(*map + i) + j))
	    ++size;
      fprintf(file, "  (%d\n", size);
      for(j = MAX_Y-1; j >= 0; --j) {
	for(i = 0; i < MAX_X; ++i) {
	  if(*(*(*map + i) + j)) {
	    fprintf(file, "    (%d,%d,%d)\n", i,j,*(*(*map + i) + j));
	  } 
	}
      }
      fprintf(file, "))\n");
    }
  }
}

int map_is_block(map_t* map, int x, int y)
{
  if(x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y)
    return 0;
  return (*map)[x][y];
}
