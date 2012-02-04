#include <stdlib.h>

#include "SDL_dep.h"

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

#ifdef __APPLE__
#define MAIN SDL_Main
#else
#define MAIN main
#endif

#define FULLSCREEN 1

#define VFOV 90.0
#define RATE_OF_CHANGE 1.0
#define POS_X 3.5
#define POS_Y 5.5

#define INDEX_XY(x,y) \
  (x * height + y)

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

  width = framebuffer_get()->w;
  height = framebuffer_get()->h;

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
      int data = (float)rand() / RAND_MAX <= 0.05;
      /* int data = i == 2 && j == 4; */
      (*map)[i][j] = data;
      
    }
}

uint32_t make_rgb(float r, float g, float b, SDL_PixelFormat* fmt)
{
  uint32_t color, red, green, blue;
  static const uint32_t white = 0xffffffff;
  red = (white & fmt->Rmask) >> fmt->Rshift;
  green = (white & fmt->Gmask) >> fmt->Gshift;
  blue = (white & fmt->Bmask) >> fmt->Bshift;
  
  red *= r;
  green *= g;
  blue *= b;
  
  color = red << fmt->Rshift;
  color += green << fmt->Gshift;
  color += blue << fmt->Bshift;
  return color;
}

void render_scene(void) {
  vec2_t* intersections;
  vec2_type* dists;
  double fps;
  int width, height;
  unsigned short x;
  char string[128];
  uint32_t* pixel_buffer;
  SDL_Surface* framebuffer;

  framebuffer = framebuffer_get();
  pixel_buffer = framebuffer->pixels;

  update_scene();
  fps = 1.0 / time_elapsed(&span);
  sprintf(string, "FPS: %5.1f", fps);

  width = framebuffer->w;
  height = framebuffer->h;

/*   Clear the buffer */
  framebuffer_wipe();

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
    for(y = (bottom_y < 0) ? 0 : bottom_y; y < top_y && y < height; ++y) {
      pixel_buffer[y*width+x] = make_rgb(1.0,1.0,1.0,framebuffer->format);
    }
  }
  mvaddstr(0,0,string);

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
  endwin();
  exit(0);
}
