#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

Array *_array_new(size_t stride) {
  Array *array = malloc(sizeof(Array));
  array->capacity = 0;
  array->size = 0;
  array->stride = stride;
  array->items = NULL;

  return array;
}

void array_free(Array *array) {
  if (array != NULL) {
    if (array->items != NULL) {
      free(array->items);
    }
    free(array);
  }
}

void *array_get(Array *array, size_t index) {
  return (uint8_t *) array->items + array->stride * index;
}

void *array_first(Array *array) {
  if (array->size > 0) {
    return array->items;
  }
  return NULL;
}

void *array_last(Array *array) {
  if (array->size > 0) {
    return (uint8_t *) array->items + array->stride * (array->size - 1);
  }
  return NULL;
}

void _array_resize(Array *array) {
  array->capacity = array->capacity == 0 ? ARRAY_DEFAULT_CAPACITY : array->capacity * 2;
  array->items = realloc(array->items, array->stride * array->capacity);
}

void _array_push(Array *array, void *value) {
  if (array->size + 1 > array->capacity) {
    _array_resize(array);
  }

  memcpy((uint8_t *) array->items + array->stride * array->size, value, array->stride);
  array->size++;
}

void *array_pop(Array *array) {
  if (array->size == 0) {
    return NULL;
  }

  void *copy = malloc(array->stride);
  memcpy(copy, array->items, array->stride);

  array_remove_first(array);

  return copy;
}

void _array_set(Array *array, size_t index, void *value) {
  size_t overflow = index + 1;
  while (overflow > array->capacity) {
    _array_resize(array);
  }
  if (overflow > array->size) {
    memset((uint8_t *) array->items + array->stride * array->size, 0, array->stride * (index - array->size));
    array->size = overflow;
  }
  memcpy((uint8_t *) array->items + array->stride * index, value, array->stride);
}

inline void _array_add_first(Array *array, void *value) {
  _array_insert(array, 0, value);
}

void _array_insert(Array *array, size_t index, void *value) {
  if (array->size + 1 > array->capacity) {
    _array_resize(array);
  }

  uint8_t *insertp = (uint8_t *) array->items + array->stride * index;
  memmove(insertp + array->stride, insertp, array->stride * (array->size - index));
  memcpy(insertp, value, array->stride);
  array->size++;
}

void array_move_first(Array *array, size_t index) {
  void *tmp = malloc(sizeof(array->stride));
  memcpy(tmp, (uint8_t *) array->items + array->stride * index, array->stride);

  memmove((uint8_t *) array->items + array->stride, array->items, array->stride * index);
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

  uint8_t *destp = (uint8_t *) array->items + array->stride * index;
  memmove(destp, destp + array->stride, array->stride * (array->size - index));
}

// utils

int array_cmp(Array *array, size_t i, size_t j) {
  return memcmp((uint8_t *) array->items + array->stride * i, (uint8_t *) array->items + array->stride * j, array->stride);
}

Array *array_concat(Array *a, Array *b) {
  if (a->stride != b->stride) {
    fprintf(
      stderr,
      "Error: trying to contact an Array<T> with sizeof(%zu) with an Array<!T> with sizeof(%zu)\n",
      a->stride,
      b->stride
    );
    return NULL;
  }

  Array *concat = _array_new(a->stride);
  for (size_t i = 0; i < a->size; i++) {
    void *value = array_get(a, i);
    _array_push(concat, value);
  }
  for (size_t i = 0; i < b->size; i++) {
    void *value = array_get(b, i);
    _array_push(concat, value);
  }

  return concat;
}

Array *array_dup(Array *array) {
  Array *dup = _array_new(array->stride);
  for (size_t i = 0; i < array->size; i++) {
    _array_push(dup, array_get(array, i));
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

void int_array_print(Array *array) {
  if (array->stride != sizeof(int)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<int> with sizeof(%zu) from an Array<!int> with sizeof(%zu)\n",
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
      "Error: trying to print an Array<long> with sizeof(%zu) from an Array<!long> with sizeof(%zu)\n",
      sizeof(long),
      array->stride
    );
    return;
  }

  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    long item = long_array_get(array, i);
    if (i == array->size - 1) {
      printf(" %ld", item);
    } else {
      printf(" %ld,", item);
    }
  }
  printf(" ] }\n");
}

void ll_array_print(Array *array) {
  if (array->stride != sizeof(long long)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<long long> with sizeof(%zu) from an Array<!long long> with sizeof(%zu)\n",
      sizeof(long long),
      array->stride
    );
    return;
  }

  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    long long item = ll_array_get(array, i);
    if (i == array->size - 1) {
      printf(" %lld", item);
    } else {
      printf(" %lld,", item);
    }
  }
  printf(" ] }\n");
}

void llu_array_print(Array *array) {
  if (array->stride != sizeof(unsigned long long)) {
    fprintf(
      stderr,
      "Error: trying to print an Array<long long> with sizeof(%zu) from an Array<!long long> with sizeof(%zu)\n",
      sizeof(unsigned long long),
      array->stride
    );
    return;
  }

  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    unsigned long long item = llu_array_get(array, i);
    if (i == array->size - 1) {
      printf(" %llu", item);
    } else {
      printf(" %llu,", item);
    }
  }
  printf(" ] }\n");
}
