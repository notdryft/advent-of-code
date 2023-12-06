#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "string.h"

StringArray *string_array_new() {
  StringArray *array = (StringArray *) malloc(sizeof(StringArray));
  array->capacity = 0;
  array->size = 0;
  array->stride = sizeof(char **);
  array->items = NULL;

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
    array->items = realloc(array->items, array->stride * array->capacity);
  }

  char *copy = string_copy(value);
  array->items[array->size++] = copy;
}

// pretty printer

void string_array_print(StringArray *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [", array->capacity, array->size, array->stride);
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

// utils

char *string_copy(char *content) {
  size_t content_len = strlen(content);
  char *content_copy = (char *) malloc(sizeof(char) * (content_len + 1));
  strncpy(content_copy, content, content_len);
  content_copy[content_len] = '\0';

  return content_copy;
}

char *substring(char *content, size_t so, size_t len) {
  char result[len + 1];
  strncpy(result, content + so, len);
  result[len] = '\0';

  return string_copy(result);
}

StringArray *string_split(char *content, char *delimiter) {
  char *copy = string_copy(content);
  StringArray *strings = string_array_new();

  char *token = strtok(copy, delimiter);
  while (token != NULL) {
    string_array_push(strings, token);

    token = strtok(NULL, delimiter);
  }

  free(copy);

  return strings;
}