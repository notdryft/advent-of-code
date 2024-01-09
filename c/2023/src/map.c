#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

Map *_map_new(size_t key_stride, size_t entry_stride) {
  Map *map = malloc(sizeof(Map));
  map->capacity = MAP_CAPACITY;
  map->table = calloc(map->capacity, sizeof(Array *));

  map->key_stride = key_stride;
  map->entry_stride = entry_stride;

  return map;
}

void map_free(Map *map) {
  if (map != NULL) {
    if (map->table != NULL) {
      for (size_t i = 0; i < map->capacity; i++) {
        Array *array = map->table[i];
        if (array != NULL) {
          array_free(array);
        }
      }
      free(map->table);
    }
    free(map);
  }
}

// gets

// https://en.wikipedia.org/wiki/Jenkins_hash_function#one_at_a_time
uint32_t _map_hash(Map *map, const void *entry) {
  const uint8_t *key = (uint8_t *) entry;

  uint32_t hash = 0;
  for (size_t i = 0; i < map->key_stride; i++) {
    hash += key[i];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;

  return hash % map->capacity;
}

void *map_get(Map *map, void *entry) {
  uint32_t hash = _map_hash(map, entry);

  Array *htable = map->table[hash];
  if (htable != NULL) {
    for (size_t i = 0; i < htable->size; i++) {
      void *item = array_get(htable, i);
      if (memcmp(entry, item, map->key_stride) == 0) {
        return item;
      }
    }
  }

  return NULL;
}

bool map_contains_key(Map *map, void *entry) {
  uint32_t hash = _map_hash(map, entry);

  Array *htable = map->table[hash];
  if (htable != NULL) {
    for (size_t i = 0; i < htable->size; i++) {
      void *item = array_get(htable, i);
      if (memcmp(entry, item, map->key_stride) == 0) {
        return true;
      }
    }
  }

  return false;
}

// sets

void map_put(Map *map, void *entry) {
  uint32_t hash = _map_hash(map, entry);
  if (map->table[hash] == NULL) {
    map->table[hash] = _array_new(map->entry_stride);
  }

  size_t index = 0;
  Array *htable = map->table[hash];
  while (index < htable->size) {
    void *item = array_get(map->table[hash], index);
    if (memcmp(entry, item, map->key_stride) == 0) {
      break;
    }
    index++;
  }

  _array_set(map->table[hash], index, entry);
}
