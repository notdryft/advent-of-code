#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "commons.h"
#include "string.h"

typedef struct {
  char lens[32];
  int focal;
  int slot;
} BoxItem;

typedef struct {
  size_t capacity;
  size_t size;
  Array **arrays;
} Map;

int hash(char *str, size_t len) {
  int h = 0;
  for (size_t i = 0; i < len; i++) {
    h += (int) str[i];
    h *= 17;
    h %= 256;
  }

  return h;
}

Map *map_new(void) {
  Map *map = malloc(sizeof(*map));
  map->capacity = 256;
  map->size = 0;
  map->arrays = malloc(sizeof(*map->arrays) * map->capacity);
  for (size_t i = 0; i < map->capacity; i++) {
    map->arrays[map->size++] = array_new(BoxItem);
  }

  return map;
}

int map_array_find(Array *array, char *instruction, size_t len) {
  for (size_t i = 0; i < array->size; i++) {
    BoxItem *item = array_get(array, i);
    if (strncmp(item->lens, instruction, len) == 0) {
      return i;
    }
  }
  return -1;
}

void map_put(Map *map, char *instruction, size_t len, int focal) {
  int h = hash(instruction, len);
  Array *array = map->arrays[h];
  int index = map_array_find(array, instruction, len);
  if (index != -1) {
    BoxItem *item = array_get(array, index);
    item->focal = focal;
  } else {
    BoxItem item = {
      .focal = focal
    };
    memset(item.lens, 0, sizeof(char) * 32);
    strncpy(item.lens, instruction, len + 1);

    array_push_rval(array, item);
  }
}

void map_remove(Map *map, char *instruction, size_t len) {
  int h = hash(instruction, len);
  Array *array = map->arrays[h];
  int index = map_array_find(array, instruction, len);
  if (index != -1) {
    array_remove(array, index);
  }
}

void map_print(Map *map) {
  for (size_t i = 0; i < map->size; i++) {
    Array *array = map->arrays[i];
    if (array->size > 0) {
      printf("Box %zu:", i);
      for (size_t j = 0; j < array->size; j++) {
          BoxItem *item = array_get(array, j);
          printf(" [%s %d]", item->lens, item->focal);
      }
      printf("\n");
    }
  }
}

void map_free(Map *map) {
  for (size_t i = 0; i < map->size; i++) {
    array_free(map->arrays[i]);
  }
  free(map->arrays);
  free(map);
}

int part1(StringArray *lines) {
  int result = 0;

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, ",");
    debug("instructions = %zu\n", split->size);
    for (size_t i = 0; i < split->size; i++) {
      char *instruction = string_array_get(split, i);
      size_t len = strlen(instruction);
      int h = hash(instruction, len);
      debug("%s (%zu) = %d\n", instruction, len, h);
      result += h;
    }
    string_array_free(split);
  }

  return result;
}

int part2(StringArray *lines) {
  Map *map = map_new();

  string_array_foreach (char *line, lines) {
    StringArray *split = string_split(line, ",");
    debug("instructions = %zu\n", split->size);

    for (size_t i = 0; i < split->size; i++) {
      char *instruction_raw = string_array_get(split, i);
      size_t len = strlen(instruction_raw);

      char *instruction = strdup(instruction_raw);
      if (instruction[len - 2] == '=') {
        instruction[len - 2] = '\0';
        int focal = atoi(instruction + len - 1);
        len -= 2;
        debug("After \"%s=%d\" (%zu):\n", instruction, focal, len);

        map_put(map, instruction, len, focal);
      } else if (instruction[len - 1] == '-') {
        instruction[len - 1] = '\0';
        len--;
        debug("After \"%s-\" (%zu):\n", instruction, len);

        map_remove(map, instruction, len);
      }

      debugf(map_print, map);
      debug("\n");

      free(instruction);
    }
    string_array_free(split);
  }

  int result = 0;
  for (size_t i = 0; i < map->size; i++) {
    Array *array = map->arrays[i];
    for (size_t j = 0; j < array->size; j++) {
      BoxItem *item = array_get(array, j);
      int power = (i + 1) * (j + 1) * item->focal;
      //debug("result += %d\n", power);
      result += power;
    }
  }

  map_free(map);

  return result;
}

int main(void) {
  test_case(day15, part1, sample, 1320);
  test_case(day15, part1, data, 514639);
  test_case(day15, part2, sample, 145);
  test_case(day15, part2, data, 279470);

  return 0;
}
