#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "array.h"

Array *_array_new(size_t stride) {
  Array *array = malloc(sizeof(*array));
  array->capacity = 0;
  array->size = 0;
  array->stride = stride;
  array->items = nullptr;

  return array;
}

void array_free(Array *array) {
  if (array != nullptr) {
    if (array->items != nullptr) {
      free(array->items);
    }
    free(array);
  }
}

void *array_get(Array *array, size_t index) {
  return (u8 *) array->items + array->stride * index;
}

void *array_first(Array *array) {
  if (array->size > 0) {
    return array->items;
  }
  return nullptr;
}

void *array_last(Array *array) {
  if (array->size > 0) {
    return (u8 *) array->items + array->stride * (array->size - 1);
  }
  return nullptr;
}

void _array_resize(Array *array) {
  array->capacity = array->capacity == 0 ? ARRAY_DEFAULT_CAPACITY : array->capacity * 2;
  array->items = realloc(array->items, array->stride * array->capacity);
}

void array_push(Array *array, void *value) {
  if (array->size + 1 > array->capacity) {
    _array_resize(array);
  }

  memcpy((u8 *) array->items + array->stride * array->size, value, array->stride);
  array->size++;
}

void *array_pop(Array *array) {
  if (array->size == 0) {
    return nullptr;
  }

  void *copy = malloc(array->stride);
  memcpy(copy, array->items, array->stride);

  array_remove_first(array);

  return copy;
}

void array_set(Array *array, size_t index, void *value) {
  size_t overflow = index + 1;
  while (overflow > array->capacity) {
    _array_resize(array);
  }
  if (overflow > array->size) {
    memset((u8 *) array->items + array->stride * array->size, 0, array->stride * (index - array->size));
    array->size = overflow;
  }
  memcpy((u8 *) array->items + array->stride * index, value, array->stride);
}

inline void array_add_first(Array *array, void *value) {
  array_insert(array, 0, value);
}

void array_insert(Array *array, size_t index, void *value) {
  if (array->size + 1 > array->capacity) {
    _array_resize(array);
  }

  u8 *insertp = (u8 *) array->items + array->stride * index;
  memmove(insertp + array->stride, insertp, array->stride * (array->size - index));
  memcpy(insertp, value, array->stride);
  array->size++;
}

void array_move_first(Array *array, size_t index) {
  void *tmp = malloc(sizeof(array->stride));
  memcpy(tmp, (u8 *) array->items + array->stride * index, array->stride);

  memmove((u8 *) array->items + array->stride, array->items, array->stride * index);
  memcpy(array->items, tmp, array->stride);

  free(tmp);
}

inline void array_remove_first(Array *array) {
  array_remove(array, 0);
}

inline void array_remove_last(Array *array) {
  array->size--;
}

void array_remove(Array *array, size_t index) {
  array->size--;

  u8 *destp = (u8 *) array->items + array->stride * index;
  memmove(destp, destp + array->stride, array->stride * (array->size - index));
}

// utils

int array_cmp(Array *array, size_t i, size_t j) {
  return memcmp((u8 *) array->items + array->stride * i, (u8 *) array->items + array->stride * j, array->stride);
}

Array *array_concat(Array *a, Array *b) {
  if (a->stride != b->stride) {
    fprintf(
      stderr,
      "Error: trying to contact an Array<T> with sizeof(%zu) with an Array<!T> with sizeof(%zu)\n",
      a->stride,
      b->stride
    );
    return nullptr;
  }

  Array *concat = _array_new(a->stride);
  for (size_t i = 0; i < a->size; i++) {
    void *value = array_get(a, i);
    array_push(concat, value);
  }
  for (size_t i = 0; i < b->size; i++) {
    void *value = array_get(b, i);
    array_push(concat, value);
  }

  return concat;
}

Array *array_dup(Array *array) {
  Array *dup = _array_new(array->stride);
  for (size_t i = 0; i < array->size; i++) {
    array_push(dup, array_get(array, i));
  }
  return dup;
}

void array_sort(Array *array, int (* cmp)(const void *, const void *)) {
  qsort(array->items, array->size, array->stride, cmp);
}

bool int_array_all(Array *array, int expected) {
  for (size_t i = 0; i < array->size; i++) {
    if (int_array_get(array, i) != expected) {
      return false;
    }
  }
  return true;
}

bool int_array_contains(Array *array, int expected) {
  for (size_t i = 0; i < array->size; i++) {
    if (int_array_get(array, i) == expected) {
      return true;
    }
  }
  return false;
}

// pretty printers

array_print(int, int, "d")
array_print(long, long, "ld")
array_print(ll, long long, "lld")
array_print(llu, unsigned long long, "llu")
array_print(i64, uint64_t, PRId64)
