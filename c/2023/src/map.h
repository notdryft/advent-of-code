#ifndef MAP
#define MAP

#include <stdbool.h>

#include "array.h"

#define MAP_CAPACITY 131071

typedef struct {
  size_t capacity;
  size_t entry_stride;
  size_t key_stride;
  size_t size;
  Array **table;
} Map;

Map *_map_new(size_t key_stride, size_t entry_stride);
#define map_new(key_type, entry_type) _map_new(sizeof(key_type), sizeof(entry_type))

void map_free(Map *map);

// gets

void *map_get(Map *map, void *key);
bool map_contains_key(Map *map, void *key);

// sets

void map_put(Map *map, void *entry);

#endif
