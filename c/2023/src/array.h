#ifndef ARRAY
#define ARRAY

#define ARRAY_DEFAULT_CAPACITY 2

typedef struct _array {
  size_t capacity;
  size_t size;
  int *data;
} Array;

Array *array_new();
void array_free(Array *array);

//

int array_get(Array *array, size_t index);
int array_first(Array *array);
int array_last(Array *array);

//

void array_push(Array *array, int value);

//

void array_print(Array *array);

#endif
