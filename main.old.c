#include "opengl_dep.h"
#include "vec2.h"
#include "map.h"
#include "game.h"
#include "raycaster.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

void transform_map(map_t* map);

int main()
{

  { /* test for the mapping part */  
    map_t* map = malloc(sizeof(map_t));
    int x,y;
    x=3, y=3; 
    memcpy(map, map_get(), sizeof(map_t));
    transform_map(map);
    map_print(map);
    printf("\n");
    
    memcpy(map_get(), map, sizeof(map_t));
    free(map);
    printf("%d,%d is a block: %d\n", x, y, map_is_block(map_get(), x, y));
  }
  {
    vec2_t intersection;
    size_t i, counter, tmp_i;

    const vec2_type theta = 90.0 / 360.0 * 2.0 * 3.14159;
    vec2_t* intersections;
    struct timeval tim;
    double t1;
    double t2;

    gettimeofday(&tim, NULL);
    
    i = 2560;
    state.cam_pos.x = 3.5;
    state.cam_pos.y = 5.5;
    state.cam_dir.x = -1.0;
    state.cam_dir.y = 0.0;
    printf("Cam Position: (%f,%f)\n", state.cam_pos.x, state.cam_pos.y);
    printf("Cam Direction: (%f,%f)\n", state.cam_dir.x, state.cam_dir.y);
    
    intersections = malloc(sizeof(vec2_t) * (i+1));
    intersections[i].x = -1.0;
    intersections[i].y = -1.0;

    t1 = tim.tv_sec + tim.tv_usec/1000000.0;
    counter = 1;
    tmp_i = 0;
    for(tmp_i = 0; tmp_i < counter; ++tmp_i) {
      raycaster_cast_rays(&state.cam_pos, &state.cam_dir, &theta,
      			  intersections,i);
    }
    gettimeofday(&tim, NULL);
    t2 = tim.tv_sec + tim.tv_usec / 1000000.0;
    printf( "Casting %d rays took %g seconds!\n", 
	    i, (t2 - t1) / (double)counter);
    
    free(intersections);
    printf("\n");
    intersection.x = -1.0;
    intersection.y = -1.0;
    if(raycaster_cast(&state.cam_pos, &state.cam_dir, &intersection)) {
      printf("Intersection at (%f, %f)\n", intersection.x, intersection.y); 
    }
    else {
      printf("No intersection!\n");
    }
  }
  free(map_get());
  return 0;
}

void transform_map(map_t* map)
{
  int i, j;
  for(i = 1; i < MAX_X - 1; ++i)
    for(j = 1; j < MAX_Y - 1; ++j) {
      /*int data = rand() % 2;*/
      int data = i == 2 && j == 4;
      (*map)[i][j] = data;
      
    }
}
