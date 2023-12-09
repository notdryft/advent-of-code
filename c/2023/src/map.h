#ifndef MAP
#define MAP

#include <stdbool.h>

#define MAP_DEFAULT_CAPACITY 2

typedef struct {
  char *key;
  void *value;
} MapEntry;

typedef struct {
  size_t capacity;
  size_t size;
  MapEntry **entries;
} Map;

Map *map_new();
void map_free(Map *map);

// gets

void *map_get(Map *map, char *key);
bool map_contains_key(Map *map, char *key);

// sets

void map_put(Map *map, char *key, void *value);

// pretty printers

void map_print(Map *map);
void int_map_print(Map *map);
void string_map_print(Map *map);

#endif
