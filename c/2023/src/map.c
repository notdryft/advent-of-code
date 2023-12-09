#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

Map *map_new() {
  Map *map = (Map *) malloc(sizeof(Map));
  map->capacity = 0;
  map->size = 0;
  map->entries = NULL;

  return map;
}

void map_free(Map *map) {
  if (map != NULL) {
    if (map->entries != NULL) {
      for (size_t i = 0; i < map->size; i++) {
        MapEntry *entry = map->entries[i];
        if (entry != NULL) {
          if (entry->key != NULL) {
            free(entry->key);
          }
          free(entry);
        }
      }
      free(map->entries);
    }
    free(map);
  }
}

// gets

void *map_get(Map *map, char *key) {
  for (size_t i = 0; i < map->size; i++) {
    MapEntry *entry = map->entries[i];
    if (strcmp(entry->key, key) == 0) {
      return entry->value;
    }
  }
  return NULL;
}

bool map_contains_key(Map *map, char *key) {
  for (size_t i = 0; i < map->size; i++) {
    MapEntry *entry = map->entries[i];
    if (strcmp(entry->key, key) == 0) {
      return true;
    }
  }
  return false;
}

// sets

void map_put(Map *map, char *key, void *value) {
  if (map->size + 1 > map->capacity) {
    map->capacity = map->capacity == 0 ? MAP_DEFAULT_CAPACITY : map->capacity * 2;
    map->entries = (MapEntry **) realloc(map->entries, sizeof(MapEntry *) * map->capacity);
  }

  MapEntry *entry = (MapEntry *) malloc(sizeof(MapEntry));
  entry->key = strdup(key);
  entry->value = value;

  map->entries[map->size++] = entry;
}

// pretty printers

void map_print(Map *map) {
  printf("Map{ capacity = %zu, size = %zu, data = [", map->capacity, map->size);
  for (size_t i = 0; i < map->size; i++) {
    MapEntry *entry = map->entries[i];
    if (i == map->size - 1) {
      printf(" { key = \"%s\", value = %p }", entry->key, entry->value);
    } else {
      printf(" { key = \"%s\", value = %p },", entry->key, entry->value);
    }
  }
  printf(" ] }\n");
}

void int_map_print(Map *map) {
  printf("Map{ capacity = %zu, size = %zu, data = [", map->capacity, map->size);
  for (size_t i = 0; i < map->size; i++) {
    MapEntry *entry = map->entries[i];
    if (i == map->size - 1) {
      printf(" { key = \"%s\", value = %d }", entry->key, *(int *) entry->value);
    } else {
      printf(" { key = \"%s\", value = %d },", entry->key, *(int *) entry->value);
    }
  }
  printf(" ] }\n");
}

void string_map_print(Map *map) {
  printf("Map{ capacity = %zu, size = %zu, data = [", map->capacity, map->size);
  for (size_t i = 0; i < map->size; i++) {
    MapEntry *entry = map->entries[i];
    if (i == map->size - 1) {
      printf(" { key = \"%s\", value = \"%s\" }", entry->key, (char *) entry->value);
    } else {
      printf(" { key = \"%s\", value = \"%s\" },", entry->key, (char *) entry->value);
    }
  }
  printf(" ] }\n");
}
