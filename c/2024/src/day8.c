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
  i64 x;
  i64 y;
} Vec2;

void vec2_array_print(Array *array) {
  printf("Array{ capacity = %zu, size = %zu, stride = %zu, data = [ ", array->capacity, array->size, array->stride);
  for (size_t i = 0; i < array->size; i++) {
    Vec2 *v = array_get(array, i);
    if (i == array->size - 1) {
      printf("(%lld, %lld)", v->x, v->y);
    } else {
      printf("(%lld, %lld), ", v->x, v->y);
    }
  }
  printf(" ] }\n");
}

constexpr size_t ANTENNAS = 256;

i64 part1(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  i64 result = 0;

  Array **antennas = malloc(sizeof(*antennas) * ANTENNAS);
  for (size_t i = 0; i < ANTENNAS; i++) {
    antennas[i] = array_new(Vec2);
  }

  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] != '.') {
        size_t antenna = grid[y][x];
        array_push(antennas[antenna], &(Vec2){ .x = x, .y = y });
      }
    }
  }

#ifdef DEBUG
  for (size_t i = 0; i < ANTENNAS; i++) {
    Array *array = antennas[i];
    if (array->size > 0) {
      printf("%c: ", (char) i);
      vec2_array_print(array);
    }
  }
#endif

  bool **antinodes = malloc(sizeof(*antinodes) * rows);
  for (size_t j = 0; j < rows; j++) {
    antinodes[j] = calloc(cols, sizeof(**antinodes));
  }

  for (size_t i = 0; i < ANTENNAS; i++) {
    Array *array = antennas[i];
    if (array->size > 0) {
      for (size_t j = 0;  j < array->size; j++) {
        for (size_t k = j + 1; k < array->size; k++) {
          Vec2 *u = array_get(array, j);
          Vec2 *v = array_get(array, k);

          Vec2 uv = { .x = v->x - u->x, .y = v->y - u->y };
          Vec2 a = { .x = u->x - uv.x, .y = u->y - uv.y};
          Vec2 b = { .x = v->x + uv.x, .y = v->y + uv.y};

          if (0 <= a.x && (size_t) a.x < cols && 0 <= a.y && (size_t) a.y < rows) {
            if (!antinodes[a.y][a.x]) {
              antinodes[a.y][a.x] = true;
              result++;
            }
          }
          if (0 <= b.x && (size_t) b.x < cols && 0 <= b.y && (size_t) b.y < rows) {
            if (!antinodes[b.y][b.x]) {
              antinodes[b.y][b.x] = true;
              result++;
            }
          }
        }
      }
    }
  }

#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == '.' && antinodes[y][x]) {
        printf("#");
      } else {
        printf("%c", grid[y][x]);
      }
    }
    printf("\n");
  }
#endif

  for (size_t i = 0; i < ANTENNAS; i++) {
    array_free(antennas[i]);
  }
  free(antennas);
  for (size_t j = 0; j < rows; j++) {
    free(antinodes[j]);
  }
  free(antinodes);

  return result;
}

i64 part2(StringArray *lines) {
  size_t cols = strlen(string_array_get(lines, 0));
  size_t rows = lines->size;
  char **grid = lines->items;

  i64 result = 0;

  Array **antennas = malloc(sizeof(*antennas) * ANTENNAS);
  for (size_t i = 0; i < ANTENNAS; i++) {
    antennas[i] = array_new(Vec2);
  }

  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] != '.') {
        size_t antenna = grid[y][x];
        array_push(antennas[antenna], &(Vec2){ .x = x, .y = y });
      }
    }
  }

#ifdef DEBUG
  for (size_t i = 0; i < ANTENNAS; i++) {
    Array *array = antennas[i];
    if (array->size > 0) {
      printf("%c: ", (char) i);
      vec2_array_print(array);
    }
  }
#endif

  bool **antinodes = malloc(sizeof(*antinodes) * rows);
  for (size_t j = 0; j < rows; j++) {
    antinodes[j] = calloc(cols, sizeof(**antinodes));
  }

  for (size_t i = 0; i < ANTENNAS; i++) {
    Array *array = antennas[i];
    if (array->size > 0) {
      for (size_t j = 0;  j < array->size; j++) {
        for (size_t k = j + 1; k < array->size; k++) {
          Vec2 *u = array_get(array, j);
          Vec2 *v = array_get(array, k);
          Vec2 uv = { .x = v->x - u->x, .y = v->y - u->y };

          Vec2 a = { .x = u->x + uv.x, .y = u->y + uv.y };
          while (0 <= a.x && (size_t) a.x < cols && 0 <= a.y && (size_t) a.y < rows) {
            if (!antinodes[a.y][a.x]) {
              antinodes[a.y][a.x] = true;
              result++;
            }
            a.x += uv.x;
            a.y += uv.y;
          }

          Vec2 b = { .x = v->x - uv.x, .y = v->y - uv.y };
          while (0 <= b.x && (size_t) b.x < cols && 0 <= b.y && (size_t) b.y < rows) {
            if (!antinodes[b.y][b.x]) {
              antinodes[b.y][b.x] = true;
              result++;
            }
            b.x -= uv.x;
            b.y -= uv.y;
          }
        }
      }
    }
  }

#ifdef DEBUG
  for (size_t y = 0; y < rows; y++) {
    for (size_t x = 0; x < cols; x++) {
      if (grid[y][x] == '.' && antinodes[y][x]) {
        printf("#");
      } else {
        printf("%c", grid[y][x]);
      }
    }
    printf("\n");
  }
#endif

  for (size_t i = 0; i < ANTENNAS; i++) {
    array_free(antennas[i]);
  }
  free(antennas);
  for (size_t j = 0; j < rows; j++) {
    free(antinodes[j]);
  }
  free(antinodes);

  return result;
}

int main(void) {
  test_case(day8, part1, sample, 14);
  test_case(day8, part1, data, 341);
  test_case(day8, part2, sample, 34);
  test_case(day8, part2, data, 1134);

  return 0;
}
