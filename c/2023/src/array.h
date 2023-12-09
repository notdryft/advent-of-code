#ifndef ARRAY
#define ARRAY

#include <stddef.h>

#define ARRAY_DEFAULT_CAPACITY 2

typedef struct {
  size_t capacity;
  size_t size;
  size_t stride;
  void **items;
} Array;

Array *_array_new(size_t stride);
#define array_new(type) _array_new(sizeof(type))

void array_free(Array *array);

// gets

void *_array_get(Array *array, size_t index);
#define int_array_get(array, index) *(int *) _array_get(array, index)
#define long_array_get(array, index) *(long *) _array_get(array, index)
#define ll_array_get(array, index) *(long long *) _array_get(array, index)

void *_array_first(Array *array);
#define int_array_first(array) *(int *) _array_first(array)
#define long_array_first(array) *(long *) _array_first(array)
#define ll_array_first(array) *(long long *) _array_first(array)

void *_array_last(Array *array);
#define int_array_last(array) *(int *) _array_last(array)
#define long_array_last(array) *(int *) _array_last(array)
#define ll_array_last(array) *(int *) _array_last(array)

// sets

void _array_push(Array *array, void *value);
#define array_push(array, value) \
  do { \
    __auto_type tmp = value; \
    _array_push(array, &tmp); \
  } while (0)

void _array_set(Array *array, size_t index, void *value);
#define array_set(array, index, value) \
  do { \
    __auto_type tmp = value; \
    _array_set(array, index, &tmp); \
  } while (0)

// pretty printers

void int_array_print(Array *array);
void long_array_print(Array *array);
void ll_array_print(Array *array);

#endif
