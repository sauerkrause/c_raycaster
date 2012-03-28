#include "controller.h"
#include <stdlib.h>
#include <memory.h>
#include <limits.h>

struct controller_s {
  int state[UCHAR_MAX];
};

controller_t* controller_new()
{
  struct controller_s * new_controller = malloc(sizeof(struct controller_s));
  memset(new_controller, 0, sizeof(controller_t));
  return new_controller;
}

int controller_is_button_down(controller_t* ctrl, int keysym)
{
  return ctrl->state[keysym];
}

int controller_is_button_up(controller_t* ctrl, int keysym)
{
  return !(ctrl->state[keysym]);
}

void controller_button_came_up(controller_t* ctrl, int keysym)
{
  ctrl->state[keysym] = 0;
}
void controller_button_went_down(controller_t* ctrl, int keysym)
{
  ctrl->state[keysym] = 1;
}
