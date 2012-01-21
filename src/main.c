#include <stdlib.h>
#include "SDL.h"
#include "SDL_main.h"
#include "vec2.h"
#include "map.h"
#include "game.h"
#include "raycaster.h"
#include "time.h"
#include "demo_state.h"
#include "framebuffer.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <ncurses.h>

#define FULLSCREEN 1

#define VFOV 90.0
#define RATE_OF_CHANGE 1.0
#define POS_X 3.5
#define POS_Y 5.5

#define MAKE_RGBA(r,g,b,a) \
  (a & 0xf) << 0 | (b & 0xf) << 4 | (g & 0xf) << 8 | (r & 0xf) << 12
#define INDEX_XY(x,y) \
  (x * height + y)

static SDL_Surface* framebuffer;

vec2_type calculate_fov(vec2_type xres, vec2_type yres, vec2_type vfov);
void render_scene(void);
void raycast_scene(vec2_t* hits, const size_t num_hits);
void update_scene(void);
void compute_cam_dists(const vec2_t* hits, const size_t num_hits,
		       vec2_type* dists);
void handle_normal_keys(unsigned char key, int x, int y);

void initialize_map(void);
void transform_map(map_t* map);
void initialize_camera(void);

static timespan span;

vec2_type calculate_fov(vec2_type xres, vec2_type yres, vec2_type vfov)
{
  vec2_type out;
  vec2_type fd;
  fd = yres * 0.5 / tan(vfov * 0.5);
  out = atan2(xres * 0.5, fd);
  return out;
}
void raycast_scene(vec2_t* hits, const size_t num_hits)
{
  vec2_type theta;
  int width, height;

  framebuffer_get_info(&width, &height, 0,0);
  theta = calculate_fov(width, height, VFOV);
  raycaster_cast_rays(&state.cam_pos, &state.cam_dir, &theta, 
		      hits, num_hits);
}

void compute_cam_dists(const vec2_t* hits, const size_t num_hits,
		       vec2_type* dists)
{
  int i;
  vec2_t negative_origin;
  vec2_t result;

  i = 0;
  for(i = 0; i < num_hits; ++i) {
    vec2_type dist;
    negative_origin = vec2_negate(&state.cam_pos);
    result = vec2_add(&hits[i], &negative_origin);
    vec2_normalize(&state.cam_dir);
    dist = vec2_dot(&state.cam_dir, &result);
    dists[i] = dist;
    /* 2.0 * sqrt(result.x * result.x + result.y * result.y); */
  }
}

void update_scene(void)
{
  vec2_rotate(& demo_state_get()->offset, 
	      RATE_OF_CHANGE * 0.15 * time_elapsed(&span));
  state.cam_pos = vec2_add(& demo_state_get()->pos, 
			   & demo_state_get()->offset);
  time_update(&span);
  vec2_rotate(&state.cam_dir, RATE_OF_CHANGE * time_elapsed(&span));
}

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
      int data = (float)rand() / RAND_MAX <= 0.15;
      /* int data = i == 2 && j == 4; */
      (*map)[i][j] = data;
      
    }
}

void render_scene(void) {
  vec2_t* intersections;
  vec2_type* dists;
  double fps;
  /* GLushort * fb; */
  GLsizei width, height;
  unsigned short x;
  char string[128];
  GLushort* pixel_buffer;
  pixel_buffer = framebuffer->pixels;

  update_scene();
  fps = 1.0 / time_elapsed(&span);
  sprintf(string, "FPS: %5.1f", fps);

  framebuffer_get_info(&width, &height, 0,0);

/*   Clear the buffer */
  memset(pixel_buffer, 0, width * height * sizeof(GLushort));

  intersections = malloc(sizeof(vec2_t) * (width + 1));
  dists = malloc(sizeof(vec2_type) * width);
  raycast_scene(intersections, width);
  compute_cam_dists(intersections, width, dists);
  free(intersections);

  for(x = 0; x < width; ++x) {
    const int midpt = height / 2;
    int y;
    int bottom_y;
    int top_y;
    vec2_type scaled_col_height;
    scaled_col_height = 1.0/dists[x];
    bottom_y = midpt - (int)(scaled_col_height * midpt);
    top_y = midpt + (int)(scaled_col_height * midpt);
    for(y = 0; y < bottom_y; ++y) {
      pixel_buffer[y*width + x] = MAKE_RGBA(0,0,0xf,0xf);
    }
    for(y = (bottom_y < 0) ? 0 : bottom_y; y < top_y && y < height; ++y) {
      pixel_buffer[y*width+x] = MAKE_RGBA(0xf,0x8,0x8,0x8);
    }
    for(y = top_y; y < height; ++y) {
      pixel_buffer[y*width+x] = MAKE_RGBA(0x0,0xf,0x0,0x0);
    }
  }
  mvaddstr(0,0,string);
  framebuffer_dump();

  refresh();
  free(dists);
  SDL_UpdateRect(framebuffer, 0,0,0,0);
}

void handle_normal_keys(unsigned char key, int x, int y)
{
  switch(key) {
  case 27:
    exit(0);
    break;
  }
}

int SDL_main(int argc, char** argv)
{
  int row, col;
  int quit;
  if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0) {
    fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
    exit(1);
  }
  framebuffer = SDL_SetVideoMode(1280,720,16,SDL_SWSURFACE);
  if(framebuffer == 0) {
    fprintf(stderr, "Unable to set 640x480 video: %s\n", SDL_GetError());
    exit(1);
  }
  
  atexit(SDL_Quit);
  row=80;
  col=24;

  initscr();
  getmaxyx(stdscr, col, row);
  row = framebuffer->w;
  col = framebuffer->h;

  initialize_camera();
  initialize_map();
  framebuffer_init(row, col-1, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4);
  map_print(map_get());
  quit = 0;
  for(;quit == 0;){
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
      case SDL_QUIT:
	quit = true;
	break;
      }
    }
    render_scene();
  }
  exit(0);
}
