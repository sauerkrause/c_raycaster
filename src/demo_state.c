#include "demo_state.h"

demo_state* demo_state_get()
{
  static demo_state demo_vars;
  return &demo_vars;
}
