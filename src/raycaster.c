#include "raycaster.h"
#include "map.h"
#include "game.h"
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
static int raycaster_cast_xplanes(const vec2_t* origin, 
				   const vec2_t* ray,
				   vec2_t* intersection);
static int raycaster_cast_yplanes(const vec2_t* origin, 
				   const vec2_t* ray,
				   vec2_t* intersection);
static vec2_type vec2_dist_from_cam_2(const vec2_t* test, const vec2_t* cam);

typedef struct rct_info
{
  const vec2_t* origin;
  const vec2_t* ray;
  const vec2_type* fov;
  size_t count;
  vec2_t* intersections;
  int id;
} rct_info;

static vec2_type vec2_dist_from_cam_2(const vec2_t* test, const vec2_t* cam)
{
  vec2_t* neg_cam;
  vec2_t* heading;
  vec2_type dot;
  
  neg_cam = malloc(sizeof(vec2_t));
  heading = malloc(sizeof(vec2_t));

  *neg_cam = vec2_negate(cam);
  *heading = vec2_add(test, neg_cam);

  dot = vec2_dot(heading, heading);

  free(neg_cam);
  free(heading);
  return pow(dot, 2.0);
}


void raycaster_cast_rays(const vec2_t* origin,
			 const vec2_t* ray,
			 const vec2_type* fov,
			 vec2_t* intersections,
			 size_t count)
{
  vec2_type lower_bound, upper_bound, d_theta;
  vec2_t bad_ray;
  int upper_ind, counter;

  upper_bound = *fov * 0.5;
  lower_bound = -upper_bound;
  d_theta = *fov / (vec2_type)count;
  
  bad_ray.x = -1.0;
  bad_ray.y = -1.0;
  
  upper_ind = (int)((upper_bound - lower_bound) / d_theta);
  
  for (counter = 0; counter < upper_ind; ++counter) {
    vec2_t current_ray;
    vec2_type theta;
    theta = counter * d_theta + lower_bound;
    memcpy(&current_ray, ray, sizeof(vec2_t));
    vec2_rotate(&current_ray, theta);
    if(!raycaster_cast(origin, &current_ray, intersections + counter))
      *((intersections) + (counter - 1)) = bad_ray;
  }
}

int raycaster_cast(const vec2_t* origin, const vec2_t* ray,
		   vec2_t* intersection)
{
  vec2_t* intersection_x;
  vec2_t* intersection_y;
  vec2_type dist_x, dist_y;
  int ret_val;

  ret_val = 0;
  intersection_x = malloc(sizeof(vec2_t));
  intersection_y = malloc(sizeof(vec2_t));
  dist_x = -1.0;
  dist_y = -1.0;
  if(!raycaster_cast_xplanes(origin, ray, intersection_x)) {
    free(intersection_x);
    intersection_x = 0;
  }
  if(!raycaster_cast_yplanes(origin, ray, intersection_y)) {
    free(intersection_y);
    intersection_y = 0;
  }

  if(intersection_y)
    dist_y = vec2_dist_from_cam_2(intersection_y, origin);
  if(intersection_x)
    dist_x = vec2_dist_from_cam_2(intersection_x, origin);

  if(dist_x < 0.0 && dist_y < 0.0) {
    ret_val = 0;
    goto raycaster_cleanup_cast_rays;
  }
  if(dist_x < 0.0) {
    memcpy(intersection, intersection_y, sizeof(vec2_t));
    ret_val = 1;
    goto raycaster_cleanup_cast_rays;
  }
  if(dist_y < 0.0) {
    memcpy(intersection, intersection_x, sizeof(vec2_t));
    ret_val = 1;
    goto raycaster_cleanup_cast_rays;
  }

  if(MIN(dist_x, dist_y) == dist_x) {
    memcpy(intersection, intersection_x, sizeof(vec2_t));
    ret_val = 1;
    goto raycaster_cleanup_cast_rays;
  } else {
    memcpy(intersection, intersection_y, sizeof(vec2_t));
    ret_val = 1;
    goto raycaster_cleanup_cast_rays;
  }

 raycaster_cleanup_cast_rays:
  if(intersection_y)
    free(intersection_y);
  if(intersection_x)
    free(intersection_x);
  intersection_x = intersection_y = 0;

  return ret_val;
}

static int raycaster_cast_xplanes(const vec2_t* origin, 
				   const vec2_t* ray,
				   vec2_t* intersection)
{
  unsigned char negative;
  vec2_t pt;
  vec2_t starter_offset;
  vec2_t offset;
  vec2_type x_off;
  int intersected;

  intersected = 0;
  if(ray->x < 0.0f)
    negative = 1;
  else
    negative = 0;

  x_off = ((int)origin->x + !negative) - origin->x;
  if(negative)
    x_off -= 0.0001f;
  starter_offset = *ray;
  offset = *ray;
  vec2_scale_x_to(&starter_offset, x_off);
  vec2_scale_x_to(&offset, negative ? -1.0 : 1.0); 
  
  pt = vec2_add(&starter_offset, origin);
  while(!intersected && pt.x >= 0.0 && pt.x < MAX_X) {
    /* Determine if the position would hit a block */
    if(map_is_block(map_get(), (int)pt.x, (int)pt.y)) {
      *intersection = pt;
      intersected = 1;
    }
    /* Continue the loop with the next intersection */
    pt = vec2_add(&offset, &pt);
  }

  return intersected;
}

static int raycaster_cast_yplanes(const vec2_t* origin, 
				   const vec2_t* ray, 
				   vec2_t* intersection)
{
  unsigned char negative;
  vec2_t pt;
  vec2_t starter_offset;
  vec2_t offset;
  vec2_type y_off;
  int intersected;

  intersected = 0;
  if(ray->y < 0.0f)
    negative = 1;
  else
    negative = 0;
  y_off = ((int)origin->y + !negative) - origin->y;
  if(negative)
    y_off -= 0.0001f;
  starter_offset = *ray;
  offset = *ray;
  vec2_scale_y_to(&starter_offset, y_off);
  vec2_scale_y_to(&offset, negative ? -1.0 : 1.0); 
  
  pt = vec2_add(&starter_offset, origin);
  while(!intersected && pt.y >= 0.0 && pt.y < MAX_Y) {
    /* Determine if the position would hit a block */
    if(map_is_block(map_get(), (int)pt.x, (int)pt.y)) {
      *intersection = pt;
      intersected = 1;
    }
    /* Continue the loop with the next intersection */
    pt = vec2_add(&offset, &pt);
  }

  return intersected;
}
