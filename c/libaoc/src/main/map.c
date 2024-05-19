#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commons.h"
#include "map.h"

Map *_map_new(size_t entry_stride, size_t key_stride) {
  Map *map = malloc(sizeof(*map));
  map->capacity = MAP_CAPACITY;
  map->table = calloc(map->capacity, sizeof(*map->table));

  map->entry_stride = entry_stride;
  map->key_stride = key_stride;
  map->size = 0;

  return map;
}

void map_free(Map *map) {
  if (map != nullptr) {
    if (map->table != nullptr) {
      for (size_t i = 0; i < map->capacity; i++) {
        Array *array = map->table[i];
        if (array != nullptr) {
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
u32 _map_hash(Map *map, const void *entry) {
  const u8 *key = (u8 *) entry;

  u32 hash = 0;
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

void *map_get(Map *map, void *key) {
  u32 hash = _map_hash(map, key);

  Array *htable = map->table[hash];
  if (htable != nullptr) {
    for (size_t i = 0; i < htable->size; i++) {
      void *item = array_get(htable, i);
      if (memcmp(key, item, map->key_stride) == 0) {
        return item;
      }
    }
  }

  return nullptr;
}

bool map_contains_key(Map *map, void *key) {
  u32 hash = _map_hash(map, key);

  Array *htable = map->table[hash];
  if (htable != nullptr) {
    for (size_t i = 0; i < htable->size; i++) {
      void *item = array_get(htable, i);
      if (memcmp(key, item, map->key_stride) == 0) {
        return true;
      }
    }
  }

  return false;
}

// sets

void map_put(Map *map, void *entry) {
  u32 hash = _map_hash(map, entry);
  if (map->table[hash] == nullptr) {
    map->table[hash] = _array_new(map->entry_stride);
  }

  bool contains_key = false;

  size_t index = 0;
  Array *htable = map->table[hash];
  while (index < htable->size) {
    void *item = array_get(map->table[hash], index);
    if (memcmp(entry, item, map->key_stride) == 0) {
      contains_key = true;
      break;
    }
    index++;
  }

  if (!contains_key) {
    array_set(map->table[hash], index, entry);
    map->size++;
  }
}
