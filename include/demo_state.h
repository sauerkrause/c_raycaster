#ifndef DEMO_STATE_H
#define DEMO_STATE_H
#include "vec2.h"

typedef struct demo_state {
  vec2_t pos;
  vec2_t offset;
} demo_state;

demo_state* demo_state_get();

#endif
