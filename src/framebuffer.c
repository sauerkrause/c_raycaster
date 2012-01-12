#include "framebuffer.h"
#include "opengl_dep.h"
#include <stdlib.h>
#include <memory.h>

#define MAKE_RGBA(r,g,b,a) \
  (a & 0xf) << 0 | (b & 0xf) << 4 | (g & 0xf) << 8 | (r & 0xf) << 12

/* Do not allow this to be set in other files */
typedef struct framebuffer_params {
  GLsizei width;
  GLsizei height;
  GLenum format;
  GLenum type;
} framebuffer_params;

static framebuffer_params parameters;

void temp_framebuffer_update(GLsizei width, GLsizei height, 
			     GLenum fmt, GLenum type, 
			     void * framebuffer)
{
  unsigned char r,g,b,a;
  unsigned short rgba_color;
  unsigned long size;
  size = width * height * sizeof(unsigned short);

/* Give the framebuffer a random color */
  r = rand() & 0xf;
  g = rand() & 0xf;
  b = rand() & 0xf;
  a = 0xf;

  rgba_color = MAKE_RGBA(r,g,b,a);
  memset(framebuffer, rgba_color, size); 
}
			     


void * framebuffer_get()
{
  static unsigned long size_fb = 0;
  static unsigned short* fb = 0;
  
/* If the framebuffer doesn't exist yet, go ahead and make it  */
/* and initialize size */
  if(!fb) {
    size_fb = parameters.height * parameters.width * sizeof(unsigned short);
    fb = malloc(size_fb);
  }

  return (void *)fb;
}

void framebuffer_get_info(GLsizei* width, GLsizei* height, 
			  GLenum* fmt, GLenum* type)
{
  if(width)
    *width = parameters.width;
  if(height)
    *height = parameters.height;
  if(fmt)
    *fmt = parameters.format;
  if(type)
    *type = parameters.type;
}

void framebuffer_init(GLsizei width, GLsizei height, GLenum fmt, GLenum type)
{
  /* glClearColor(0.0,0.0,0.0,0.0); */
  /* glShadeModel(GL_FLAT); */
  parameters.width = width;
  parameters.height = height;
  parameters.format = fmt;
  parameters.type = type;
}

void framebuffer_dump()
{
  /* glClear(GL_COLOR_BUFFER_BIT); */
  /* glRasterPos2i(0,0); */
  /* glDrawPixels(parameters.width, */
  /* 	       parameters.height, */
  /* 	       parameters.format, */
  /* 	       parameters.type, */
  /* 	       framebuffer_get()); */
  /* glFlush(); */
  memset(framebuffer_get(),
	 0x000f, parameters.width*parameters.height*sizeof(GLushort));
}
