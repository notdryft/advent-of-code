#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

void int_array_tests() {
  Array *ints = int_array_new();
  int_array_print(ints);

  Array *longs = array_new(sizeof(long));
  int_array_print(longs);

  int_array_push(ints, 7);
  int_array_print(ints);
  printf("first = %d, last = %d\n", int_array_first(ints), int_array_last(ints));

  int_array_push(ints, 3);
  int_array_print(ints);
  printf("first = %d, last = %d\n", int_array_first(ints), int_array_last(ints));

  printf("result = [ %d, %d ]\n", int_array_get(ints, 0), int_array_get(ints, 1));

  array_free(ints);
  array_free(longs);
}

int main() {
  int_array_tests();

  return 0;
}
