#ifndef ARRAY
#define ARRAY

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

void *_array_get(Array *array, size_t index);
#define array_get(array, index) *(char *) _array_get(array, index)

void *_array_first(Array *array);
#define array_first(array) *(char *) _array_first(array)

void *_array_last(Array *array);
#define array_last(array) *(char *) _array_last(array)

// sets

void _array_push(Array *array, void *value);
#define array_push(array, value) \
  do { \
    __auto_type tmp = value; \
    _array_push(array, &tmp); \
  } while (0)

// pretty printer

void int_array_print(Array *array);

#endif
