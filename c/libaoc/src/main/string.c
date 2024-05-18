#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

StringArray *string_array_new(void) {
  StringArray *array = malloc(sizeof(StringArray));
  array->capacity = 0;
  array->size = 0;
  array->stride = sizeof(char *);
  array->items = nullptr;

  return array;
}

void string_array_free(StringArray *array) {
  for (size_t i = 0; i < array->size; i++) {
    free(array->items[i]);
  }
  free(array->items);
  free(array);
}

// gets

char *string_array_get(StringArray *array, size_t index) {
  return array->items[index];
}

char *string_array_first(StringArray *array) {
  return array->items[0];
}

char *string_array_last(StringArray *array) {
  return array->items[array->size - 1];
}

// sets

void string_array_push(StringArray *array, char *value) {
  if (array->size + 1 > array->capacity) {
    array->capacity = array->capacity == 0 ? ARRAY_DEFAULT_CAPACITY : array->capacity * 2;
    array->items = (char **) realloc(array->items, array->stride * array->capacity);
  }

  char *copy = strdup(value);
  array->items[array->size++] = copy;
}

void *string_array_pop(StringArray *array) {
  if (array->size == 0) {
    return nullptr;
  }

  char *copy = strdup(array->items[0]);
  string_array_remove_first(array);

  return copy;
}

void string_array_remove_first(StringArray *array) {
  string_array_remove(array, 0);
}

void string_array_remove_last(StringArray *array) {
  free(array->items[--array->size]);
}

void string_array_remove(StringArray *array, size_t index) {
  free(array->items[index]);
  array->size--;

  memmove(&array->items[index], &array->items[index + 1], array->stride * (array->size - index));
  array->items[array->size] = nullptr;
}

// utils

char *repeat(char *content, size_t times) {
  size_t len = strlen(content);
  size_t repeated_len = len * times + 1;
  char *repeated = malloc(sizeof(char) * repeated_len);
  memset(repeated, 0, sizeof(char) * repeated_len);
  for (size_t i = 0; i < times; i++) {
    strcat(repeated, content);
  }
  return repeated;
}

char *repeat_with_delimiter(char *content, size_t times, char *delimiter) {
  size_t len = strlen(content);
  size_t delimiter_len = strlen(delimiter);
  size_t repeated_len = len * times + delimiter_len * times;
  char *repeated = malloc(sizeof(char) * repeated_len);
  memset(repeated, 0, sizeof(char) * repeated_len);
  for (size_t i = 0; i < times; i++) {
    strcat(repeated, content);
    if (i < times - 1) {
      strcat(repeated, delimiter);
    }
  }
  return repeated;
}

char *substring(char *content, size_t so, size_t len) {
  char *result = calloc(len + 1, sizeof(*result));
  strncpy(result, content + so, len);
  return result;
}

bool string_array_contains(StringArray *array, char *value) {
  for (size_t i = 0; i < array->size; i++) {
    if (strcmp(array->items[i], value) == 0) {
      return true;
    }
  }
  return false;
}

StringArray *string_array_dup(StringArray *array) {
  StringArray *dup = string_array_new();
  for (size_t i = 0; i < array->size; i++) {
    string_array_push(dup, string_array_get(array, i));
  }
  return dup;
}

StringArray *string_array_repeat(StringArray *array, size_t times) {
  StringArray *repeated = string_array_new();
  for (size_t i = 0; i < times; i++) {
    for (size_t j = 0; j < array->size; j++) {
      string_array_push(repeated, string_array_get(array, j));
    }
  }
  return repeated;
}

StringArray *string_array_subset(StringArray *array, size_t start, size_t end) {
  StringArray *subset = string_array_new();
  for (size_t i = start; i < end; i++) {
    string_array_push(subset, string_array_get(array, i));
  }
  return subset;
}

StringArray *string_split(char *content, char *delimiter) {
  char *copy = strdup(content);
  StringArray *strings = string_array_new();

  char *token = strstr(copy, delimiter);
  if (token == nullptr) {
     string_array_push(strings, copy);
  } else {
    size_t delimiter_len = strlen(delimiter);
    char *p = copy;
    while ((token = strstr(p, delimiter)) != nullptr) {
      *token = '\0';
      string_array_push(strings, p);
      p = token + delimiter_len;
    }
    string_array_push(strings, p);
  }

  free(copy);

  return strings;
}

Array *string_atoi(StringArray *array) {
  Array *ints = array_new(int);
  for (size_t i = 0; i < array->size; i++) {
    array_push_rval(ints, atoi(string_array_get(array, i)));
  }
  return ints;
}

Array *string_atol(StringArray *array) {
  Array *longs = array_new(long);
  for (size_t i = 0; i < array->size; i++) {
    array_push_rval(longs, atol(string_array_get(array, i)));
  }
  return longs;
}

Array *string_atoll(StringArray *array) {
  Array *lls = array_new(long long);
  for (size_t i = 0; i < array->size; i++) {
    array_push_rval(lls, atoll(string_array_get(array, i)));
  }
  return lls;
}

// pretty printer

void string_array_print(StringArray *array) {
  printf("StringArray{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    char *item = string_array_get(array, i);
    if (i == array->size - 1) {
      printf(" \"%s\"", item);
    } else {
      printf(" \"%s\",", item);
    }
  }
  printf(" ] }\n");
}

void string_array_print_raw(StringArray *array) {
  for (size_t i = 0; i < array->size; i++) {
    char *item = string_array_get(array, i);
    printf("%s\n", item);
  }
}
