#ifndef TIME
#define TIME

#include <inttypes.h>

#include "commons.h"

typedef struct {
  uint64_t seconds;
  uint64_t milliseconds;
  uint64_t microseconds;
} Time;

struct timeval now();

Time interval(struct timeval start, struct timeval end);

#endif
