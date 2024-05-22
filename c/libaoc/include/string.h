#ifndef STRING
#define STRING

#include <string.h>

#include "array.h"

typedef struct {
  size_t capacity;
  size_t size;
  size_t stride;
  char **items;
} StringArray;

StringArray *string_array_new(void);
void string_array_free(StringArray *array);

// gets

char *string_array_get(StringArray *array, size_t index);
char *string_array_first(StringArray *array);
char *string_array_last(StringArray *array);

// sets

void string_array_push(StringArray *array, char *value);

void *string_array_shift(StringArray *array);
void *string_array_pop(StringArray *array);

void string_array_remove_first(StringArray *array);
void string_array_remove_last(StringArray *array);

void string_array_remove(StringArray *array, size_t index);

// utils

char *repeat(char *content, size_t times);
char *repeat_with_delimiter(char *content, size_t times, char *delimiter);
char *substring(char *content, size_t pos, size_t len);

bool string_array_contains(StringArray *array, char *value);
StringArray *string_array_dup(StringArray *array);
StringArray *string_array_repeat(StringArray *array, size_t times);
StringArray *string_array_subset(StringArray *array, size_t start, size_t end);
StringArray *string_split(char *content, char *delimiter);

Array *string_atoi(StringArray *array);
Array *string_atol(StringArray *array);
Array *string_atoll(StringArray *array);

// pretty printer

void string_array_print(StringArray *array);
void string_array_print_raw(StringArray *array);

#endif
