#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "opengl_dep.h"

#define PIXEL_TYPE GL_UNSIGNED_SHORT_4_4_4_4
#define PIXEL_FORMAT GL_RGBA
#define FB_WIDTH 320
#define FB_HEIGHT 240

void temp_framebuffer_update(GLsizei width, GLsizei height, 
			     GLenum fmt, GLenum type,
			     void* framebuffer);

/* retrieves the cpu-side framebuffer */
void * framebuffer_get();

/* retrieves the info used to initialize the framebuffer */
void framebuffer_get_info(GLsizei* width, GLsizei* height, 
			  GLenum* fmt, GLenum* type);

/* initializes clear color and shade model */
/* Takes width,  */
/* height of the framebuffer,  */
/* pixel format, */
/* type used per pixel */
void framebuffer_init(GLsizei width, GLsizei height, GLenum fmt, GLenum type);

/* shoves the cpu-side framebuffer to the gpu */
void framebuffer_dump();

#endif
