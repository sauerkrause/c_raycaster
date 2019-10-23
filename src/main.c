#include "SDL_dep.h"

#include "vec2.h"
#include "map.h"
#include "game.h"
#include "raycaster.h"
#include "time.h"
#include "demo_state.h"
#include "framebuffer.h"
#include "renderer.h"
#include "controller.h"
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

#define WALK_SPEED 5.0f
#define TURN_ANGLE 2.0f

void transform_map(map_t* map);

void initialize_camera(void)
{
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

int moving_forward = 0;

controller_t* controller;

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
      case SDLK_q:
	*quit = true;
	break;
      default:
	controller_button_went_down(controller, event.key.keysym.sym);
	break;
      }
      break;
    case SDL_KEYUP:
      controller_button_came_up(controller, event.key.keysym.sym);
      break;	
    }
  }

  /* Process game logic */
  if(controller_is_button_down(controller, SDLK_w)) {
    vec2_t velocity;
    velocity = state.cam_dir;
    vec2_scale(&velocity, WALK_SPEED * (float)span.elapsed);
    state.cam_pos = vec2_add(&state.cam_pos, &velocity);
  }
  if(controller_is_button_down(controller, SDLK_s)) {
    vec2_t velocity;
    velocity = state.cam_dir;
    vec2_scale(&velocity, -1.0);
    vec2_scale(&velocity, WALK_SPEED * (float)span.elapsed);
    state.cam_pos = vec2_add(&state.cam_pos, &velocity);
  }
  if(controller_is_button_down(controller, SDLK_d)) {
    vec2_rotate(&state.cam_dir, TURN_ANGLE * (float)span.elapsed);
    vec2_normalize(&state.cam_dir);
  }
  if(controller_is_button_down(controller, SDLK_a)) {
    vec2_rotate(&state.cam_dir, -1.0f * TURN_ANGLE * (float)span.elapsed);
    vec2_normalize(&state.cam_dir);
  }

  /* Render view */
  renderer_render_scene();
}

int main(int argc, char** argv)
{
  int quit;
  SDL_Surface * framebuffer;
  controller = controller_new();

  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  framebuffer = SDL_SetVideoMode(320,240,32,SDL_SWSURFACE);
  
  if(framebuffer == 0) {
    fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
    exit(1);
  }

  framebuffer_set(framebuffer);
  
  atexit(SDL_Quit);

  initscr();
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
