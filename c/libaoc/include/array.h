#ifndef ARRAY
#define ARRAY

#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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
void *array_get_pointer(Array *array, size_t index);
#define int_array_get(array, index) *(int *) array_get(array, index)
#define long_array_get(array, index) *(long *) array_get(array, index)
#define ll_array_get(array, index) *(long long *) array_get(array, index)
#define llu_array_get(array, index) *(unsigned long long *) array_get(array, index)
#define i64_array_get(array, index) *(int64_t *) array_get(array, index)

void *array_first(Array *array);
#define int_array_first(array) *(int *) array_first(array)
#define long_array_first(array) *(long *) array_first(array)
#define ll_array_first(array) *(long long *) array_first(array)
#define llu_array_first(array) *(unsigned long long *) array_first(array)
#define i64_array_first(array) *(int64_t *) array_first(array)

void *array_last(Array *array);
void *array_last_pointer(Array *array);
#define int_array_last(array) *(int *) array_last(array)
#define long_array_last(array) *(long *) array_last(array)
#define ll_array_last(array) *(long long *) array_last(array)
#define llu_array_last(array) *(unsigned long long *) array_last(array)
#define i64_array_last(array) *(int64_t *) array_last(array)

void array_push(Array *array, void *value);
void array_push_pointer(Array *array, void *value);
#define array_push_rval(array, value) \
  do { \
    __auto_type tmp = value; \
    array_push(array, &tmp); \
  } while (0)

void *array_shift(Array *array);
void *array_pop(Array *array);

void array_set(Array *array, size_t index, void *value);
#define array_set_rval(array, index, value) \
  do { \
    __auto_type tmp = value; \
    array_set(array, index, &tmp); \
  } while (0)

void array_add_first(Array *array, void *value);
#define array_add_first_rval(array, value) \
  do { \
    __auto_type tmp = value; \
    array_add_first(array, &tmp); \
  } while (0)

void array_add_last(Array *array, void *value);
#define array_add_last_rval(array, value) \
  do { \
    __auto_type tmp = value; \
    array_push(array, &tmp); \
  } while (0)

void array_insert(Array *array, size_t index, void *value);
#define array_insert_rval(array, index, value) \
  do { \
    __auto_type tmp = value; \
    array_insert(array, index, &tmp); \
  } while (0)

void array_move_first(Array *array, size_t index);

void array_remove_first(Array *array);
void array_remove_last(Array *array);

void array_remove(Array *array, size_t index);

// utils

int array_cmp(Array *array, size_t i, size_t j);
Array *array_concat(Array *a, Array *b);
Array *array_dup(Array *array);
void array_sort(Array *array, int (*cmp)(const void *, const void *));

bool int_array_all(Array *array, int expected);
bool int_array_contains(Array *array, int expected);

// pretty printers

#define array_print_signature(prefix, type) void prefix##_array_print(Array *array)
#define array_print(prefix, type, format) \
  void prefix##_array_print(Array *array) { \
    if (array->stride != sizeof(type)) { \
      fprintf( \
        stderr, \
        "Error: trying to print an Array<" #type "> with sizeof(%zu) from an Array<!" #type "> with sizeof(%zu)\n", \
        sizeof(type), \
        array->stride \
      ); \
      return; \
    } \
    printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride); \
    for (size_t i = 0; i < array->size; i++) { \
      type item = *(type *) array_get(array, i); \
      if (i == array->size - 1) { \
        printf(" %" format, item); \
      } else { \
        printf(" %" format ",", item); \
      } \
    } \
    printf(" ] }\n"); \
  }

array_print_signature(int, int);
array_print_signature(long, long);
array_print_signature(ll, long long);
array_print_signature(llu, unsigned long long);
array_print_signature(i64, int64_t);

#endif
