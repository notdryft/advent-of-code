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

void string_array_tests() {
  Array *strings = string_array_new();
  string_array_print(strings);

  string_array_push(strings, "seven");
  string_array_print(strings);
  printf("first = \"%s\", last = \"%s\"\n", string_array_first(strings), string_array_last(strings));

  string_array_push(strings, "three");
  string_array_print(strings);
  printf("first = \"%s\", last = \"%s\"\n", string_array_first(strings), string_array_last(strings));

  printf("result = [ \"%s\", \"%s\" ]\n", string_array_get(strings, 0), string_array_get(strings, 1));

  array_free(strings);
}

int main() {
  printf("// ints\n");
  int_array_tests();

  printf("// strings\n");
  string_array_tests();

  return 0;
}
