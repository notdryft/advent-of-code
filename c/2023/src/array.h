#ifndef ARRAY
#define ARRAY

#include <stdbool.h>
#include <stddef.h>

#define ARRAY_DEFAULT_CAPACITY 2

typedef struct {
  size_t capacity;
  size_t size;
  size_t stride;
  void *items;
} Array;

Array *_array_new(size_t stride);
#define array_new(type) _array_new(sizeof(type))

void array_free(Array *array);

// gets

void *array_get(Array *array, size_t index);
#define int_array_get(array, index) *(int *) array_get(array, index)
#define long_array_get(array, index) *(long *) array_get(array, index)
#define ll_array_get(array, index) *(long long *) array_get(array, index)
#define llu_array_get(array, index) *(unsigned long long *) array_get(array, index)

void *array_first(Array *array);
#define int_array_first(array) *(int *) array_first(array)
#define long_array_first(array) *(long *) array_first(array)
#define ll_array_first(array) *(long long *) array_first(array)
#define llu_array_first(array) *(unsigned long long *) array_first(array)

void *array_last(Array *array);
#define int_array_last(array) *(int *) array_last(array)
#define long_array_last(array) *(long *) array_last(array)
#define ll_array_last(array) *(long long *) array_last(array)
#define llu_array_last(array) *(unsigned long long *) array_last(array)

void _array_push(Array *array, void *value);
#define array_push(array, value) \
  do { \
    __auto_type tmp = value; \
    _array_push(array, &tmp); \
  } while (0)

void *array_pop(Array *array);

void _array_set(Array *array, size_t index, void *value);
#define array_set(array, index, value) \
  do { \
    __auto_type tmp = value; \
    _array_set(array, index, &tmp); \
  } while (0)

void _array_add_first(Array *array, void *value);
#define array_add_first(array, value) \
  do { \
    __auto_type tmp = value; \
    _array_add_first(array, &tmp); \
  } while (0)

void _array_add_last(Array *array, void *value);
#define array_add_last(array, value) \
  do { \
    __auto_type tmp = value; \
    _array_push(array, &tmp); \
  } while (0)

void _array_insert(Array *array, size_t index, void *value);
#define array_insert(array, index, value) \
  do { \
    __auto_type tmp = value; \
    _array_insert(array, index, &tmp); \
  } while (0)

void array_move_first(Array *array, size_t index);

void array_remove_first(Array *array);
void array_remove_last(Array *array);

void array_remove(Array *array, size_t index);

// utils

int array_cmp(Array *array, size_t i, size_t j);
Array *array_dup(Array *array);

bool int_array_all(Array *array, int expected);
bool int_array_contains(Array *array, int expected);

// pretty printers

void int_array_print(Array *array);
void long_array_print(Array *array);
void ll_array_print(Array *array);
void llu_array_print(Array *array);

#endif
