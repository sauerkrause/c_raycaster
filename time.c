#include "time.h"
#include <sys/time.h>

/* Updates the internal state of the timespan. */
void time_update(timespan* span)
{
  struct timeval tim;
  gettimeofday(&tim, 0);
  span->last = span->current;
  span->current = (double)tim.tv_sec + (tim.tv_usec / 1000000.0);
  span->elapsed = span->current - span->last;
}

/* Returns the time elapsed as of the last update */
double time_elapsed(timespan* span)
{
  return span->elapsed;
}
