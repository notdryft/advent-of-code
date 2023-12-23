#include <stdio.h>
#include <stdlib.h>

#include "algos.h"
#include "array.h"

Cycle *find_longest_cycle(Array *array, size_t min_period) {
  for (size_t i = 0; i < array->size - 1; i++) {
    for (size_t j = i + 1; j < array->size; j++) {
      if ((array->size - i) % (j - i) == 0) {
        size_t ii = i, jj = j;
        while (jj < array->size && array_cmp(array, ii, jj) == 0) {
          ii++;
          jj++;
        }
        if (jj == array->size && jj - ii > min_period) {
          Cycle *c = malloc(sizeof(Cycle));
          c->start = i;
          c->period = j - i;
          return c;
        }
      }
    }
  }
  return NULL;
}
