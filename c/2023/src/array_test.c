#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

void int_array_tests(void) {
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

  array_add_first(ints, 11);
  int_array_print(ints);

  array_insert(ints, 5, 12);
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
    Item *item = array_get(array, i);
    if (i == array->size - 1) {
      printf("{ a = %d, b = %d }", item->a, item->b);
    } else {
      printf("{ a = %d, b = %d }, ", item->a, item->b);
    }
  }
  printf(" ] }\n");
}

void dyn_array_tests(void) {
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

  array_add_first(items, ((Item) { .a = 13, .b = 14 }));
  item_array_print(items);

  array_insert(items, 2, ((Item) { .a = 15, .b = 16 }));
  array_remove_first(items);
  item_array_print(items);

  array_insert(items, 4, ((Item) { .a = 17, .b = 18 }));
  array_remove_first(items);
  item_array_print(items);

  array_move_first(items, 2);
  item_array_print(items);

  array_free(items);
}

void array_priority_push(Array *array, Item value) {
  if (array->size == 0) {
    printf("push\n");
    array_push(array, value);
  } else {
    bool stop = false;
    size_t i = 0;
    Item *other = array_get(array, i++);
    while (other->a < value.a && i <= array->size) {
      other = array_get(array, i++);
    }
    printf("insert at %zu (%zu)\n", i - 1, array->size);
    array_insert(array, i - 1, value);
  }
}

void array_priority_push_tests(void) {
  Array *items = array_new(Item);
  item_array_print(items);

  array_priority_push(items, ((Item) { .a = 3, .b = 4 }));
  item_array_print(items);

  array_priority_push(items, ((Item) { .a = 1, .b = 2 }));
  item_array_print(items);

  array_remove_first(items);
  item_array_print(items);

  array_remove_first(items);
  item_array_print(items);

  array_priority_push(items, ((Item) { .a = 9, .b = 10 }));
  array_priority_push(items, ((Item) { .a = 5, .b = 6 }));
  array_priority_push(items, ((Item) { .a = 11, .b = 12 }));
  array_priority_push(items, ((Item) { .a = 7, .b = 8 }));
  item_array_print(items);

  array_remove(items, 2);
  item_array_print(items);

  array_priority_push(items, ((Item) { .a = 13, .b = 14 }));
  item_array_print(items);

  array_priority_push(items, ((Item) { .a = 15, .b = 16 }));
  array_remove_first(items);
  item_array_print(items);

  array_priority_push(items, ((Item) { .a = 17, .b = 18 }));
  array_remove_first(items);
  item_array_print(items);

  array_free(items);
}

int main(void) {
  printf("// ints\n");
  int_array_tests();

  printf("// dyn array\n");
  dyn_array_tests();

  printf("// priority queue\n");
  array_priority_push_tests();

  return 0;
}
