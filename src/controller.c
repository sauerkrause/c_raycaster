#include "controller.h"
#include <stdlib.h>
#include <memory.h>

struct controller_s {
  int state[256];
};

controller_t* controller_new()
{
  controller_t* new_controller = malloc(sizeof(controller_t));
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
