#ifndef CONTROLLER_H
#define CONTROLLER_H

struct controller_s;

typedef struct controller_s controller_t;

controller_t* controller_new(void);

int controller_is_button_down(controller_t* ctrl, int keysym);
int controller_is_button_up(controller_t* ctrl, int keysym);

void controller_button_went_down(controller_t* ctrl, int keysym);
void controller_button_came_up(controller_t* ctrl, int keysym);

#endif
