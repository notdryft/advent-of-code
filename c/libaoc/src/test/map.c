#include <stdio.h>
#include <string.h>

#include "commons.h"
#include "map.h"

typedef struct {
  size_t key1;
  size_t key2;
  i32 key3;
  unsigned long long value;
} MapEntry;

constexpr size_t KEY_STRIDE = sizeof(size_t) * 2 + sizeof(i32);

void map_print(Map *map) {
  printf("Map{\n");
  for (size_t i = 0; i < map->capacity; i++) {
    Array *array = map->table[i];
    if (array != nullptr) {
      printf("  0x%05lx -> [ ", i);
      for (size_t j = 0; j < array->size; j++) {
        MapEntry *entry = array_get(array, j);
        if (j == array->size - 1) {
          printf("{ %zu, %zu, %d, %llu }", entry->key1, entry->key2, entry->key3, entry->value);
        } else {
          printf("{ %zu, %zu, %d, %llu }, ", entry->key1, entry->key2, entry->key3, entry->value);
        }
      }
      printf(" ]\n");
    }
  }
  printf("}\n");
}

void map_tests(void) {
  Map *map = map_new(MapEntry, KEY_STRIDE);
  map_print(map);

  size_t len = 10;
  for (size_t i = 0; i < len; i++) {
    for (size_t j = 0; j < len; j++) {
      for (size_t k = 0; k < len; k++) {
        for (size_t l = 0; l < len; l++) {
          MapEntry *entry = &(MapEntry) { j, k, l, i };
          printf("Contains key { %zu, %zu, %d } = %s\n", entry->key1, entry->key2, entry->key3, map_contains_key(map, &entry) ? "true" : "false");
          printf("Adding entry { %zu, %zu, %d, %llu }\n", entry->key1, entry->key2, entry->key3, entry->value);
          map_put(map, entry);
        }
      }
    }
  }
  map_print(map);

  map_free(map);
}

int main(void) {
  printf("// Map\n");
  map_tests();

  return 0;
}
