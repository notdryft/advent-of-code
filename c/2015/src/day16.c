#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

typedef struct {
  char key[32];
  i64 value;
} MapEntry;

enum Op {
  EQUAL,
  GT,
  LT
};

typedef struct {
  char key[32];
  i64 value;
  enum Op op;
} FilterEntry;

constexpr size_t KEY_STRIDE = sizeof(char) * 32;

void map_print(Map *map) {
  printf("Sue{ ");
  for (size_t i = 0; i < map->capacity; i++) {
    Array *array = map->table[i];
    if (array != nullptr) {
      for (size_t j = 0; j < array->size; j++) {
        MapEntry *entry = array_get(array, j);
        printf("%s: %lld, ", entry->key, entry->value);
      }
    }
  }
  printf("}\n");
}

void push_filter(Array *filters, char *key, i64 value, enum Op op) {
  FilterEntry filter = { .value = value, .op = op };
  strncpy(filter.key, key, 31);
  array_push_rval(filters, filter);
}

Array *create_filters() {
  Array *filters = array_new(FilterEntry);
  push_filter(filters, "children", 3, EQUAL);
  push_filter(filters, "cats", 7, GT);
  push_filter(filters, "samoyeds", 2, EQUAL);
  push_filter(filters, "pomeranians", 3, LT);
  push_filter(filters, "akitas", 0, EQUAL);
  push_filter(filters, "vizslas", 0, EQUAL);
  push_filter(filters, "goldfish", 5, LT);
  push_filter(filters, "trees", 3, GT);
  push_filter(filters, "cars", 2, EQUAL);
  push_filter(filters, "perfumes", 1, EQUAL);

  return filters;
}

i64 part1(StringArray *lines) {
  Array *sues = array_new(Map *);

  string_array_foreach (char *line, lines) {
    char key1[12], key2[12], key3[12];
    i64 number, value1, value2, value3;

    if (
      sscanf(
        line,
        "Sue %lld: %[^:]: %lld, %[^:]: %lld, %[^:]: %lld",
        &number, key1, &value1, key2, &value2, key3, &value3
      ) == 7
    ) {
      MapEntry n = { .value = number };
      strncpy(n.key, "number", 12);
      MapEntry entry1 = { .value = value1 };
      strncpy(entry1.key, key1, 12);
      MapEntry entry2 = { .value = value2 };
      strncpy(entry2.key, key2, 12);
      MapEntry entry3 = { .value = value3 };
      strncpy(entry3.key, key3, 12);

      Map *sue = map_new(MapEntry, KEY_STRIDE);
      map_put(sue, &n);
      map_put(sue, &entry1);
      map_put(sue, &entry2);
      map_put(sue, &entry3);

      array_push_pointer(sues, sue);
    }
  }
#ifdef DEBUG
  for (size_t i = 0; i < sues->size; i++) {
    Map *sue = array_get_pointer(sues, i);
    map_print(sue);
  }
#endif

  Array *filters = create_filters();
  Array *filtered = array_new(Map *);
  for (size_t i = 0; i < sues->size; i++) {
    Map *sue = array_get_pointer(sues, i);
    bool keep = true;
    for (size_t j = 0; j < filters->size && keep; j++) {
      FilterEntry *filter = array_get(filters, j);
      MapEntry *entry = map_get(sue, filter->key);
      if (entry != nullptr) {
        if (entry->value != filter->value) {
          keep = false;
        }
      }
    }
    if (keep) {
      array_push_pointer(filtered, sue);
    }
  }
  debug("filtered: %zu\n", filtered->size);
#ifdef DEBUG
  for (size_t i = 0; i < filtered->size; i++) {
    Map *sue = array_get_pointer(filtered, i);
    map_print(sue);
  }
#endif

  assert(filtered->size == 1);
  Map *sue = array_get_pointer(filtered, 0);
  MapEntry search;
  strncpy(search.key, "number", 31);
  MapEntry *number = map_get(sue, &search);
  assert(number != nullptr);
  i64 result = number->value;

  array_free(filters);
  array_free(filtered);
  for (size_t i = 0; i < sues->size; i++) {
    Map *sue = array_get_pointer(sues, i);
    map_free(sue);
  }
  array_free(sues);

  return result;
}

i64 part2(StringArray *lines) {
  Array *sues = array_new(Map *);

  string_array_foreach (char *line, lines) {
    char key1[12], key2[12], key3[12];
    i64 number, value1, value2, value3;

    if (
      sscanf(
        line,
        "Sue %lld: %[^:]: %lld, %[^:]: %lld, %[^:]: %lld",
        &number, key1, &value1, key2, &value2, key3, &value3
      ) == 7
    ) {
      MapEntry n = { .value = number };
      strncpy(n.key, "number", 12);
      MapEntry entry1 = { .value = value1 };
      strncpy(entry1.key, key1, 12);
      MapEntry entry2 = { .value = value2 };
      strncpy(entry2.key, key2, 12);
      MapEntry entry3 = { .value = value3 };
      strncpy(entry3.key, key3, 12);

      Map *sue = map_new(MapEntry, KEY_STRIDE);
      map_put(sue, &n);
      map_put(sue, &entry1);
      map_put(sue, &entry2);
      map_put(sue, &entry3);

      array_push_pointer(sues, sue);
    }
  }
#ifdef DEBUG
  for (size_t i = 0; i < sues->size; i++) {
    Map *sue = array_get_pointer(sues, i);
    map_print(sue);
  }
#endif

  Array *filters = create_filters();
  Array *filtered = array_new(Map *);
  for (size_t i = 0; i < sues->size; i++) {
    Map *sue = array_get_pointer(sues, i);
    bool keep = true;
    for (size_t j = 0; j < filters->size && keep; j++) {
      FilterEntry *filter = array_get(filters, j);
      MapEntry *entry = map_get(sue, filter->key);
      if (entry != nullptr) {
        if (filter->op == EQUAL && entry->value != filter->value) {
          keep = false;
        } else if (filter->op == GT && entry->value <= filter->value) {
          keep = false;
        } else if (filter->op == LT && entry->value >= filter->value) {
          keep = false;
        }
      }
    }
    if (keep) {
      array_push_pointer(filtered, sue);
    }
  }
  debug("filtered: %zu\n", filtered->size);
#ifdef DEBUG
  for (size_t i = 0; i < filtered->size; i++) {
    Map *sue = array_get_pointer(filtered, i);
    map_print(sue);
  }
#endif

  assert(filtered->size == 1);
  Map *sue = array_get_pointer(filtered, 0);
  MapEntry search;
  strncpy(search.key, "number", 31);
  MapEntry *number = map_get(sue, &search);
  assert(number != nullptr);
  i64 result = number->value;

  array_free(filters);
  array_free(filtered);
  for (size_t i = 0; i < sues->size; i++) {
    Map *sue = array_get_pointer(sues, i);
    map_free(sue);
  }
  array_free(sues);

  return result;
}

int main(void) {
  test_case(day16, part1, data, 103);
  test_case(day16, part2, data, 405);

  return 0;
}
