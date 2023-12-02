#ifndef ARRAY
#define ARRAY

#define ARRAY_DEFAULT_CAPACITY 2

typedef struct {
  size_t capacity;
  size_t size;
  size_t stride;
  void *items;
} Array;

Array *array_new(size_t stride);
void array_free(Array *array);

// gets

void *array_get(Array *array, size_t index);
void *array_first(Array *array);
void *array_last(Array *array);

// sets

void array_push(Array *array, void *value);

// int arrays

#define int_array_new() array_new(sizeof(int))

#define int_array_get(array, index) *(char *) array_get(array, index)
#define int_array_first(array) *(char *) array_first(array)
#define int_array_last(array) *(char *) array_last(array)

#define int_array_push(array, value) \
  do { \
    __auto_type tmp = value; \
    array_push(array, &tmp); \
  } while (0)

void int_array_print(Array *array);

#endif
