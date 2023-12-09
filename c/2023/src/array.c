#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

Array *_array_new(size_t stride) {
  Array *array = (Array *) malloc(sizeof(Array));
  array->capacity = 0;
  array->size = 0;
  array->stride = stride;
  array->items = malloc(sizeof(void **));
  array->items[0] = NULL;

  return array;
}

void array_free(Array *array) {
  if (array->items[0] != NULL) {
    free(array->items[0]);
  }
  free(array->items);
  free(array);
}

// gets

void *_array_get(Array *array, size_t index) {
  return array->items[0] + array->stride * index;
}

void *_array_first(Array *array) {
  if (array->size > 0) {
    return array->items[0];
  }
  return NULL;
}

void *_array_last(Array *array) {
  if (array->size > 0) {
    return *array->items + array->stride * (array->size - 1);
  }
  return NULL;
}

// sets

void _array_resize(Array *array) {
  array->capacity = array->capacity == 0 ? ARRAY_DEFAULT_CAPACITY : array->capacity * 2;
  array->items[0] = realloc(array->items[0], array->stride * array->capacity);
}

void _array_push(Array *array, void *value) {
  if (array->size + 1 > array->capacity) {
    _array_resize(array);
  }

  memcpy(array->items[0] + array->stride * array->size, value, array->stride);
  array->size += 1;
}

void _array_set(Array *array, size_t index, void *value) {
  size_t overflow = index + 1;
  while (overflow > array->capacity) {
    _array_resize(array);
  }
  if (overflow > array->size) {
    memset(array->items[0] + array->stride * array->size, 0, array->stride * (index - array->size));
    array->size = overflow;
  }
  memcpy(array->items[0] + array->stride * index, value, array->stride);
}

// pretty printer

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

void long_array_print(Array *array) {
  if (array->stride != sizeof(long)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<long> with sizeof(%zu) from a Array<!long> with sizeof(%zu)\n",
      sizeof(long),
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

void ll_array_print(Array *array) {
  if (array->stride != sizeof(long long)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<long long> with sizeof(%zu) from a Array<!long long> with sizeof(%zu)\n",
      sizeof(long long),
      array->stride
    );
    return;
  }

  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    int item = ll_array_get(array, i);
    if (i == array->size - 1) {
      printf(" %d", item);
    } else {
      printf(" %d,", item);
    }
  }
  printf(" ] }\n");
}
