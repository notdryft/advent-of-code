#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

Array *array_new(size_t stride) {
  Array *array = (Array *) malloc(sizeof(Array));
  array->capacity = 0;
  array->size = 0;
  array->stride = stride;
  array->items = NULL;

  return array;
}

void array_free(Array *array) {
  free(array->items);
  free(array);
}

// gets

void *array_get(Array *array, size_t index) {
  return array->items + array->stride * index;
}

void *array_first(Array *array) {
  if (array->size > 0) {
    return array->items;
  }
  return NULL;
}

void *array_last(Array *array) {
  if (array->size > 0) {
    return array->items + array->stride * (array->size - 1);
  }
  return NULL;
}

// sets

void array_push(Array *array, void *value) {
  if (array->size + 1 > array->capacity) {
    array->capacity = array->capacity == 0 ? ARRAY_DEFAULT_CAPACITY : array->capacity * 2;
    array->items = realloc(array->items, array->stride * array->capacity);
  }

  memcpy(array->items + array->stride * array->size, value, array->stride);
  array->size += 1;
}

// ints

void int_array_print(Array *array) {
  if (array->stride != sizeof(int)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<int> with sizeof(%zu) from a Array<!int> with sizeof(%zu)\n",
      sizeof(int),
      array->stride
    );
    return;
  }

  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    int item = int_array_get(array, i);
    if (i == array->size - 1) {
      printf(" %d", item);
    } else {
      printf(" %d,", item);
    }
  }
  printf(" ] }\n");
}

// strings

void string_array_print(Array *array) {
  if (array->stride != sizeof(char *)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<char *> with sizeof(%zu) from a Array<!char *> with sizeof(%zu)\n",
      sizeof(char *),
      array->stride
    );
    return;
  }

  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    char *item = string_array_get(array, i);
    if (i == array->size - 1) {
      printf(" \"%s\"", item);
    } else {
      printf(" \"%s\"", item);
    }
  }
  printf(" ] }\n");
}
