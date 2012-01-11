#ifndef VEC2_H
#define VEC2_H
typedef float vec2_type;

typedef struct vec2 
{
  vec2_type x;
  vec2_type y;
} vec2_t;

void vec2_rotate(vec2_t* ray, vec2_type theta);
vec2_type vec2_dot(const vec2_t* ray1, const vec2_t* ray2);
void vec2_normalize(vec2_t* ray);
void vec2_scale(vec2_t* ray, vec2_type scale_fact);
vec2_t vec2_add(const vec2_t* left, const vec2_t* right);
vec2_t vec2_negate(const vec2_t* ray);

void vec2_scale_x_to(vec2_t* ray, vec2_type target);
void vec2_scale_y_to(vec2_t* ray, vec2_type target);

#endif
