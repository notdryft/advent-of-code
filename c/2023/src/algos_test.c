#include <stdio.h>
#include <stdlib.h>

#include "algos.h"
#include "array.h"
#include "string.h"

void find_longest_cycle_test(int *numbers, size_t len) {
  Array *array = array_new(int);
  for (size_t i = 0; i < len; i++) {
    array_push(array, numbers[i]);
  }
  int_array_print(array);

  Cycle *c = find_longest_cycle(array, 2);
  if (c != nullptr) {
    printf("Cycle from %zu with period %zu\n", c->start, c->period);
    free(c);
  } else {
    printf("No cycle found\n");
  }

  array_free(array);
}

void find_longest_cycle_tests(void) {
  find_longest_cycle_test((int[]) { 1, 2, 3, 4, 5, 6 }, 6);
  find_longest_cycle_test((int[]) { 1, 2, 3, 1, 2 }, 5);
  find_longest_cycle_test((int[]) { 1, 2, 3, 1, 2, 3 }, 6);
  find_longest_cycle_test((int[]) { 1, 2, 3, 4, 5, 6, 4, 5, 6 }, 9);
  find_longest_cycle_test((int[]) { 1 }, 1);
  find_longest_cycle_test((int[]) { 1, 2 }, 2);
  find_longest_cycle_test((int[]) { 1, 2, 2 }, 3);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2 }, 4);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2, 3 }, 5);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2, 3, 4 }, 6);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2, 3, 4, 2 }, 7);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2, 3, 4, 2, 2 }, 8);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2, 3, 4, 2, 2, 3 }, 9);
  find_longest_cycle_test((int[]) { 1, 2, 2, 2, 3, 4, 2, 2, 3, 4 }, 10);
}

int main(void) {
  printf("// Find longest cycle with minimum period\n");
  find_longest_cycle_tests();

  return 0;
}
