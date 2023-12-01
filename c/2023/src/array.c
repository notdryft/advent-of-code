#include <stdio.h>
#include <stdlib.h>

#include "array.h"

Array *array_new() {
  Array *array = (Array *) malloc(sizeof(Array));
  array->capacity = ARRAY_DEFAULT_CAPACITY;
  array->size = 0;
  array->data = malloc(sizeof(int) * ARRAY_DEFAULT_CAPACITY);

  return array;
}

void array_free(Array *array) {
  free(array->data);
  free(array);
}

//

int array_get(Array *array, size_t index) {
  return array->data[index];
}

void array_push(Array *array, int value) {
  if (array->size + 1 > array->capacity) {
    array->capacity = array->capacity * 2;
    array->data = realloc(array->data, sizeof(int) * array->capacity);
  }

  array->data[array->size] = value;
  array->size = array->size + 1;
}

//

void array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, data = [ ", array->capacity, array->size);
  for (size_t i = 0; i < array->size; i++) {
    if (i == array->size - 1) {
      printf("%d", array->data[i]);
    } else {
      printf("%d, ", array->data[i]);
    }
  }
  printf(" ] }\n");
}
