#include <stdlib.h>
#include "opengl_dep.h"
#include "vec2.h"
#include "map.h"
#include "game.h"
#include "raycaster.h"
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>

#define PROFILING 1

#define FULLSCREEN 1
#define XRES 2560
#define YRES 1600

#define FOV 75.0
#define RATE_OF_CHANGE 1.0
#define POS_X 3.5
#define POS_Y 5.5

/* callback to render the scene */
void render_scene(void);
void raycast_scene(vec2_t* hits, const size_t num_hits);
void update_scene(void);
void compute_cam_dists(const vec2_t* hits, const size_t num_hits,
		       vec2_type* dists);
void handle_normal_keys(unsigned char key, int x, int y);

void initialize_map(void);
void transform_map(map_t* map);
void initialize_camera(void);

vec2_t pos;
vec2_t offset;

typedef struct timespan {
  double last;
  double current;
} timespan;

timespan span;

void time_update()
{
  struct timeval tim;
  gettimeofday(&tim, 0);
  span.last = span.current;
  span.current = (double)tim.tv_sec + (double)tim.tv_usec / 1000000.0;
}

double time_elapsed()
{
  return span.current - span.last;
}

void raycast_scene(vec2_t* hits, const size_t num_hits)
{
  vec2_type theta = FOV / 360.0 * 2.0 * 3.14159;
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
  vec2_rotate(&offset, RATE_OF_CHANGE * 0.15 * time_elapsed());
  state.cam_pos = vec2_add(&pos, &offset);
  time_update();
  vec2_rotate(&state.cam_dir, RATE_OF_CHANGE * time_elapsed());
  /* printf("Cam dir: (%f,%f)\n", state.cam_dir.x, state.cam_dir.y); */
}

void initialize_camera(void)
{
  pos.x = 39.5;
  pos.y = 11.5;
  offset.x = 10;
  offset.y = 0.0;
  span.current = 0.0;
  time_update();
  time_update();
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
  size_t quad_it;
  double fps;

  update_scene();

  fps = 1.0 / time_elapsed();

  printf("FPS: %g\n", fps);

  quad_it = 0;
  intersections = malloc(sizeof(vec2_t) * (XRES + 1));
  dists = malloc(sizeof(vec2_type) * XRES);
  raycast_scene(intersections, XRES);
  compute_cam_dists(intersections, XRES, dists);
  free(intersections);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_QUADS);

  glColor3f(0.0,0.0,1.0);
  glVertex2f(-1.0, 1.0);
  glVertex2f(1.0, 1.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(-1.0, 0.0);

  glColor3f(0.0,1.0,0.0);
  glVertex2f(-1.0, 0.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(1.0, -1.0);
  glVertex2f(-1.0, -1.0);

  glColor3f(0.5,0.5,0.5);
  for(quad_it = 0; quad_it < XRES; ++quad_it) {
    const float width = 2.0 / (float)XRES;
    float height;
    
    height = 1.0 / dists[quad_it];

    glVertex2f(-1.0+width*quad_it, height);
    glVertex2f(-1.0+width*quad_it, -height);
    glVertex2f(-1.0+width*(quad_it+1), -height);
    glVertex2f(-1.0+width*(quad_it+1), height);
  }
  glEnd();
  glutSwapBuffers();
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
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100,100);
  glutInitWindowSize(XRES, YRES);
  glutCreateWindow("Raycaster Rev C");
  if(FULLSCREEN)
    glutFullScreen();

  /* register callbacks */
  glutDisplayFunc(&render_scene);
  glutIdleFunc(&render_scene);
  glutKeyboardFunc(&handle_normal_keys);
  
  initialize_camera();
  initialize_map();
  map_print(map_get());

  /* Enter GLUT event processing cycle */
  glutMainLoop();
  return 1;
}
