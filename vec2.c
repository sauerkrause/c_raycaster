#include "vec2.h"
#include <math.h>

void vec2_rotate(vec2_t* ray, vec2_type theta)
{
  float x, y, s_t, c_t;
  s_t = sin(theta);
  c_t = cos(theta);
  x = ray->x;
  y = ray->y;

  ray->x = x * c_t - y * s_t;
  ray->y = x * s_t + y * c_t;
}

vec2_type vec2_dot(const vec2_t* ray1, const vec2_t* ray2)
{
  vec2_type dot_prod;
  dot_prod = ray1->x * ray2->x + ray1->y * ray2->y;
  return dot_prod;
}

void vec2_normalize(vec2_t* ray)
{
  vec2_type scale_fact;
  scale_fact = 1.0 / sqrt(pow(vec2_dot(ray, ray),2.0));
  vec2_scale(ray, scale_fact);
}

void vec2_scale(vec2_t* ray, vec2_type scale_fact)
{
  ray->x *= scale_fact;
  ray->y *= scale_fact;
}

vec2_t vec2_add(const vec2_t* left, const vec2_t* right)
{
  vec2_t ray;

  ray.x = left->x + right->x;
  ray.y = left->y + right->y;
  return ray;
}

vec2_t vec2_negate(const vec2_t* ray)
{
  vec2_t negation; 

  negation.x = -ray->x;
  negation.y = -ray->y;
  return negation;
}

void vec2_scale_x_to(vec2_t* ray, vec2_type target)
{
  vec2_type scale_fact;

  scale_fact = target / ray->x;
  vec2_scale(ray, scale_fact);
}
void vec2_scale_y_to(vec2_t* ray, vec2_type target)
{
  vec2_type scale_fact;

  scale_fact = target / ray->y;
  vec2_scale(ray, scale_fact);
}
