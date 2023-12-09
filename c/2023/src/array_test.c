#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

void int_array_tests() {
  Array *ints = array_new(int);
  int_array_print(ints);

  Array *longs = array_new(long);
  int_array_print(longs);

  array_push(ints, 7);
  int_array_print(ints);
  printf("first = %d, last = %d\n", int_array_first(ints), int_array_last(ints));

  array_push(ints, 3);
  int_array_print(ints);
  printf("first = %d, last = %d\n", int_array_first(ints), int_array_last(ints));

  printf("result = [ %d, %d ]\n", int_array_get(ints, 0), int_array_get(ints, 1));

  array_set(ints, 10, 42);
  int_array_print(ints);

  array_free(ints);
  array_free(longs);
}

int main() {
  printf("// ints\n");
  int_array_tests();

  return 0;
}
