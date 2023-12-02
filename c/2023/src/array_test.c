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
  printf("first = %d, last = %d\n", array_first(ints), array_last(ints));

  array_push(ints, 3);
  int_array_print(ints);
  printf("first = %d, last = %d\n", array_first(ints), array_last(ints));

  printf("result = [ %d, %d ]\n", array_get(ints, 0), array_get(ints, 1));

  array_free(ints);
  array_free(longs);
}

int main() {
  printf("// ints\n");
  int_array_tests();

  return 0;
}
