#ifndef ALGOS
#define ALGOS

#include "array.h"

typedef struct {
  size_t start;
  size_t period;
} Cycle;

Cycle *find_longest_cycle(Array *array, size_t min_period);

#endif
