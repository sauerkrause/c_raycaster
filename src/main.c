

#include "SDL_dep.h"

#include "vec2.h"
#include "map.h"
#include "game.h"
#include "raycaster.h"
#include "time.h"
#include "demo_state.h"
#include "framebuffer.h"
#include "renderer.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <ncurses.h>
#include "span.h"

#ifdef __APPLE__
#define MAIN SDL_Main
#else
#define MAIN main
#endif

void initialize_camera(void)
{
  demo_state_get()->pos.x = 39.5;
  demo_state_get()->pos.y = 11.5;
  demo_state_get()->offset.x = 10;
  demo_state_get()->offset.y = 0.0;
  span.current = 0.0;
  time_update(&span);
  time_update(&span);
  state.cam_pos.x = 3.5;
  state.cam_pos.y = 3.5;
  state.cam_dir.x = -1.0;
  state.cam_dir.y = 0.0;
}

void initialize_map(void)
{
  FILE * out; 
  out = fopen("map.ss", "r");
  if(!out) {
    transform_map(map_get());
    out = fopen("map.ss", "w");
    map_serialize(map_get(), out, 1);
  } else {
    map_serialize(map_get(), out, 0);
  }
  fclose(out);
}

void transform_map(map_t* map)
{
  int i, j;
  for(i = 1; i < MAX_X - 1; ++i)
    for(j = 1; j < MAX_Y - 1; ++j) {
      int data = (float)rand() / RAND_MAX <= 0.05;
      /* int data = i == 2 && j == 4; */
      (*map)[i][j] = data;
      
    }
}


void handle_normal_keys(unsigned char key, int x, int y)
{
  switch(key) {
  case 27:
    exit(0);
    break;
  }
}

void loop(int* quit)
{
  /* Handle controls */
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
    case SDL_QUIT:
      *quit = true;
      break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
      case SDLK_ESCAPE:
      case 'q':
	*quit = true;
	break;
      }
	
    }
  }
  /* Process game logic */

  /* Render view */
  renderer_render_scene();
}

int main(int argc, char** argv)
{
  int row, col;
  int quit;
  SDL_Surface * framebuffer;

  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  framebuffer = SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
  
  if(framebuffer == 0) {
    fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
    exit(1);
  }

  framebuffer_set(framebuffer);
  
  atexit(SDL_Quit);
  row=80;
  col=24;

  initscr();
  getmaxyx(stdscr, col, row);
  row = framebuffer->w;
  col = framebuffer->h;

  initialize_camera();
  initialize_map();

  /* map_print(map_get()); */
  quit = 0;
  for(;quit == 0;){
    loop(&quit);
  }
  endwin();
  exit(0);
}
