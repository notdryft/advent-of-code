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

typedef struct {
  int a;
  int b;
} Item;

void item_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Item *item = (Item *) array_get(array, i);
    if (i == array->size - 1) {
      printf("{ a = %d, b = %d }", item->a, item->b);
    } else {
      printf("{ a = %d, b = %d }, ", item->a, item->b);
    }
  }
  printf(" ] }\n");
}

void dyn_array_tests() {
  Array *items = array_new(Item);
  item_array_print(items);

  array_push(items, ((Item) { .a = 1, .b = 2 }));
  item_array_print(items);

  array_push(items, ((Item) { .a = 3, .b = 4 }));
  item_array_print(items);

  array_remove_first(items);
  item_array_print(items);

  array_remove_first(items);
  item_array_print(items);

  array_push(items, ((Item) { .a = 5, .b = 6 }));
  array_push(items, ((Item) { .a = 7, .b = 8 }));
  array_push(items, ((Item) { .a = 9, .b = 10 }));
  array_push(items, ((Item) { .a = 11, .b = 12 }));
  item_array_print(items);

  array_remove(items, 2);
  item_array_print(items);

  array_free(items);
}

int main() {
  printf("// ints\n");
  int_array_tests();

  printf("// dyn array\n");
  dyn_array_tests();

  return 0;
}
