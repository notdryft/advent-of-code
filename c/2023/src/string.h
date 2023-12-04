#ifndef STRING
#define STRING

#include <string.h>

typedef struct {
  size_t capacity;
  size_t size;
  size_t stride;
  char **items;
} StringArray;

StringArray *string_array_new();
void string_array_free(StringArray *array);

// gets

char *string_array_get(StringArray *array, size_t index);
char *string_array_first(StringArray *array);
char *string_array_last(StringArray *array);

// sets

void string_array_push(StringArray *array, char *value);

// pretty printer

void string_array_print(StringArray *array);

// utils

char *string_copy(char *content);
char *substring(char *content, size_t pos, size_t len);
StringArray *string_split(char *content, char *delimiter);

#endif
