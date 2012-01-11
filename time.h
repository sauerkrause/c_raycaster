#ifndef TIME_H
#define TIME_H

typedef struct timespan {
  double last;
  double current;
  double elapsed;
} timespan;

void time_update(timespan* span);

double time_elapsed(timespan* span);

#endif
