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
  i32 x;
  i32 y;
} Vec2;

#define vec2_get(map, ...) map_get(map, &(Vec2) { __VA_ARGS__ })
#define vec2_contains_key(map, ...) map_contains_key(map, &(Vec2) { __VA_ARGS__ })

#define vec2_put(map, ...) map_put(map, &(Vec2) { __VA_ARGS__ })

void map_print(Map *map) {
  printf("Map{\n");
  for (size_t i = 0; i < map->capacity; i++) {
    Array *array = map->table[i];
    if (array != nullptr) {
      printf("  0x%05lx -> [ ", i);
      for (size_t j = 0; j < array->size; j++) {
        Vec2 *vec = array_get(array, j);
        if (j == array->size - 1) {
          printf("{ %d, %d }", vec->x, vec->y);
        } else {
          printf("{ %d, %d }, ", vec->x, vec->y);
        }
      }
      printf(" ]\n");
    }
  }
  printf("} size = %zu\n", map->size);
}

i64 part1(StringArray *lines) {
  Map *map = map_new(Vec2, sizeof(Vec2));

  i32 x = 0, y = 0;
  vec2_put(map, .x = x, .y = y);

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    for (size_t j = 0; j < line_len; j++) {
      char c = line[j];

      if (c == '^') y -= 1;
      else if (c == '>') x += 1;
      else if (c == 'v') y += 1;
      else if (c == '<') x -= 1;

      trace("%c:\n", c);
      vec2_put(map, .x = x, .y = y);
      tracef(map_print, map);
    }
  }

  i64 result = map->size;
  map_free(map);

  return result;
}

i64 part2(StringArray *lines) {
  Map *map = map_new(Vec2, sizeof(Vec2));

  i32 sx = 0, sy = 0;
  i32 rx = 0, ry = 0;
  vec2_put(map, .x = sx, .y = sy);

  bool santa = true;

  string_array_foreach (char *line, lines) {
    size_t line_len = strlen(line);
    debug("%s: %zu\n", line, line_len);

    for (size_t j = 0; j < line_len; j++) {
      char c = line[j];
      trace("%c:\n", c);

      if (santa) {
        if (c == '^') sy -= 1;
        else if (c == '>') sx += 1;
        else if (c == 'v') sy += 1;
        else if (c == '<') sx -= 1;

        vec2_put(map, .x = sx, .y = sy);
      } else {
        if (c == '^') ry -= 1;
        else if (c == '>') rx += 1;
        else if (c == 'v') ry += 1;
        else if (c == '<') rx -= 1;

        vec2_put(map, .x = rx, .y = ry);
      }

      tracef(map_print, map);

      santa = !santa;
    }
  }

  i64 result = map->size;
  map_free(map);

  return result;
}

int main(void) {
  test_case(day3, part1, sample, 4);
  test_case(day3, part1, data, 2592);
  test_case(day3, part2, data, 2360);

  return 0;
}
