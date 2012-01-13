#include <stdlib.h>
#include "opengl_dep.h"
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


/* callback to render the scene */
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
  transform_map(map_get());
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
  GLushort * fb;
  GLsizei width, height;
  unsigned short x;
  char string[128];

  update_scene();
  fps = 1.0 / time_elapsed(&span);
  sprintf(string, "FPS: %5.1f", fps);

  framebuffer_get_info(&width, &height, 0,0);

  intersections = malloc(sizeof(vec2_t) * (width + 1));
  dists = malloc(sizeof(vec2_type) * width);
  raycast_scene(intersections, width);
  compute_cam_dists(intersections, width, dists);
  free(intersections);

  fb = framebuffer_get();

  for(x = 0; x < width; ++x) {
    const int midpt = height / 2;
    int y;
    int bottom_y;
    int top_y;
    vec2_type scaled_col_height;
    scaled_col_height = 1.0/dists[x];
    bottom_y = midpt - (int)(scaled_col_height * midpt);
    top_y = midpt + (int)(scaled_col_height * midpt);
    attron(COLOR_PAIR(1));
    for(y = 0; y < bottom_y; ++y) {
      const GLushort pix_color = MAKE_RGBA(0,0xf,0,0xf);
      fb[x*height + y] = pix_color;
      mvaddch(y,x,' ');
    }
    attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(2));
    for(y = (bottom_y < 0) ? 0 : bottom_y; y < top_y && y < height; ++y) {
      GLushort pix_color;
      pix_color = MAKE_RGBA(7,7,7,0xf);
      fb[x*height + y] = pix_color;
      mvaddch(y,x,'|');
    }
    attroff(COLOR_PAIR(2));
    attron(COLOR_PAIR(3));
    for(y = top_y; y < height; ++y) {
      const GLushort pix_color = MAKE_RGBA(0,0,0xf,0xf);
      fb[x*height+y] = pix_color;
      mvaddch(y,x,'-');
    }
    attroff(COLOR_PAIR(3));
  }

  addstr(string);
  framebuffer_dump();
  refresh();
  /* glutSwapBuffers(); */
  free(dists);
  
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
  /* glutInit(&argc, argv); */
  /* glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); */
  /* glutInitWindowPosition(100,100); */
  /* glutInitWindowSize(XRES, YRES); */
  /* glutCreateWindow("Raycaster Rev C"); */
  /* if(FULLSCREEN) */
  /*   glutFullScreen(); */

  /* /\* register callbacks *\/ */
  /* glutDisplayFunc(&render_scene); */
  /* glutIdleFunc(&render_scene); */
  /* glutKeyboardFunc(&handle_normal_keys); */
  
  initscr();
  getmaxyx(stdscr, col, row);

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_BLUE);
  init_pair(2, COLOR_WHITE, COLOR_WHITE);
  init_pair(3, COLOR_GREEN, COLOR_GREEN);

  initialize_camera();
  initialize_map();
  framebuffer_init(row, col-1, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4);
  map_print(map_get());
  for(;;){
    render_scene();
  }
  /* Enter GLUT event processing cycle */
  glutMainLoop();
  endwin();

  return 1;
}
