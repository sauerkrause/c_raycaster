#ifndef RAYCASTER_H
#define RAYCASTER_H
#include "vec2.h"
#include <stddef.h>

int raycaster_cast(const vec2_t* origin, 
		   const vec2_t* ray, 
		   vec2_t* intersection);

void raycaster_cast_rays(const vec2_t* origin,
			 const vec2_t* ray,
			 const vec2_type* fov,
			 vec2_t* intersections,
			 size_t count);
#endif
