#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "math.h"
#include "map.h"
#include "string.h"

i64 part1(StringArray *lines, i64 iterations) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, " ");
    Array *stones = string_atoll(split);
    debugf(i64_array_print, stones);

    while (iterations > 0) {
      Array *new_stones = array_new(i64);
      for (size_t i = 0; i < stones->size; i++) {
        i64 n = i64_array_get(stones, i);
        if (n == 0) {
          i64 replacement = 1;
          array_push_rval(new_stones, replacement);
        } else {
          char n_str[100] = {};
          sprintf(n_str, "%lld", n);
          size_t len = strlen(n_str);
          if (len % 2 == 0) {
            size_t half = len / 2;
            char left_str[50] = {};
            char right_str[50] = {};
            #pragma GCC diagnostic ignored "-Wstringop-truncation"
            strncpy(left_str, n_str, half);
            #pragma GCC diagnostic ignored "-Wstringop-truncation"
            strncpy(right_str, n_str + half, half);
            i64 left = atoll(left_str);
            i64 right = atoll(right_str);
            array_push_rval(new_stones, left);
            array_push_rval(new_stones, right);
          } else {
            i64 replacement = n * 2024;
            array_push_rval(new_stones, replacement);
          }
        }
      }
      iterations--;
      debugf(i64_array_print, new_stones);
      
      array_free(stones);
      stones = new_stones;
    }

    result += stones->size;

    array_free(stones);
    string_array_free(split);
  }

  return result;
}

typedef struct {
  i64 key;
  i64 value;
} MapEntry;

constexpr size_t KEY_STRIDE = sizeof(i64);

Array *map_entries(Map *map) {
  Array *entries = array_new(MapEntry);
  for (size_t i = 0; i < map->capacity; i++) {
    Array *htable = map->table[i];
    if (htable != nullptr) {
      for (size_t j = 0; j < htable->size; j++) {
        MapEntry *entry = array_get(htable, j);
        array_push(entries, entry);
      }
    }
  }
  return entries;
}

void map_print(Map *map) {
  printf("Map{\n");
  for (size_t i = 0; i < map->capacity; i++) {
    Array *htable = map->table[i];
    if (htable != nullptr) {
      printf("  0x%05lx -> [ ", i);
      for (size_t j = 0; j < htable->size; j++) {
        MapEntry *entry = array_get(htable, j);
        if (j == htable->size - 1) {
          printf("{ %lld -> %lld }", entry->key, entry->value);
        } else {
          printf("{ %lld -> %lld }, ", entry->key, entry->value);
        }
      }
      printf(" ]\n");
    }
  }
  printf("}\n");
}

i64 part2(StringArray *lines, i64 iterations) {
  i64 result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, " ");

    Map *map = map_new(MapEntry, KEY_STRIDE);
    for (size_t i = 0; i < split->size; i++) {
      char *key_str = string_array_get(split, i);
      i64 key = atoll(key_str); 
      map_put(map, &(MapEntry) { .key = key, .value = 1 });
    }
    debugf(map_print, map);

    while (iterations > 0) {
      Map *updated = map_new(MapEntry, KEY_STRIDE);
      Array *entries = map_entries(map);
      for (size_t i = 0; i < entries->size; i++) {
        MapEntry *entry = array_get(entries, i);
        if (entry->key == 0) {
          i64 value = entry->value;
          MapEntry *e = map_get(updated, &(MapEntry) { .key = 1 });
          if (e != nullptr) {
            value += e->value;
          }
          map_put(updated, &(MapEntry) { .key = 1, .value = value });
        } else {
          char n_str[100] = {};
          sprintf(n_str, "%lld", entry->key);
          size_t len = strlen(n_str);
          if (len % 2 == 0) {
            size_t half = len / 2;
            char left_str[50] = {};
            char right_str[50] = {};
            #pragma GCC diagnostic ignored "-Wstringop-truncation"
            strncpy(left_str, n_str, half);
            #pragma GCC diagnostic ignored "-Wstringop-truncation"
            strncpy(right_str, n_str + half, half);
            i64 left = atoll(left_str);
            i64 lvalue = entry->value;
            MapEntry *el = map_get(updated, &(MapEntry) { .key = left });
            if (el != nullptr) {
              lvalue += el->value;
            }
            map_put(updated, &(MapEntry) { .key = left, .value = lvalue });
            i64 right = atoll(right_str);
            i64 rvalue = entry->value;
            MapEntry *er = map_get(updated, &(MapEntry) { .key = right });
            if (er != nullptr) {
              rvalue += er->value;
            }
            map_put(updated, &(MapEntry) { .key = right, .value = rvalue });
          } else {
            i64 key = 2024 * entry->key;
            i64 value = entry->value;
            MapEntry *e = map_get(updated, &(MapEntry) { .key = key });
            if (e != nullptr) {
              value += e->value;
            }
            map_put(updated, &(MapEntry) { .key = key, .value = value });
          }
        }
      }
      iterations--;

      array_free(entries);
      map_free(map);
      map = updated;

      debugf(map_print, map);
    }


    Array *entries = map_entries(map);
    for (size_t i = 0; i < entries->size; i++) {
      MapEntry *entry = array_get(entries, i);
      result += entry->value;
    }

    array_free(entries);
    map_free(map);
    string_array_free(split);
  }

  return result;
}

int main(void) {
  test_case(day11, part1, sample, 55312, 25);
  test_case(day11, part1, data, 209412, 25);
  test_case(day11, part2, sample, 65601038650482, 75);
  test_case(day11, part2, data, 248967696501656, 75);

  return 0;
}
